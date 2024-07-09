import dash
from dash.dependencies import Output, Input
from dash import dcc
from dash import html
import plotly.graph_objs as go
from collections import deque
import time
from pysnmp.hlapi import *
from dash.exceptions import PreventUpdate
from pysnmp.smi import builder, view, error

# SNMP parameters
community_string = 'public'
host = 'localhost'
port = 161

mibBuilder = builder.MibBuilder()
mibViewController = view.MibViewController(mibBuilder)
mibBuilder.loadModules('mib')

def load_mib_and_check_syntax(mib_path):
    mibBuilder = builder.MibBuilder()
    try:
        mibBuilder.loadModules(mib_path)
        print(f"MIB loaded successfully: {mib_path}")
    except error.SmiError as e:
        print(f"Error loading MIB {mib_path}: {e}")

# Substitua '/caminho/para/mib.smiv2' pelo caminho real do seu arquivo MIB
load_mib_and_check_syntax('/usr/share/snmp/mibs/mib.smiv2')

def get_snmp_data(host, port, community, oid):
    try:
        iterator = getCmd(
            SnmpEngine(),
            CommunityData(community),
            UdpTransportTarget((host, port)),
            ContextData(),
            ObjectType(ObjectIdentity(oid))
        )

        errorIndication, errorStatus, errorIndex, varBinds = next(iterator)

        if errorIndication:
            raise Exception(errorIndication)
        elif errorStatus:
            raise Exception(f'{errorStatus.prettyPrint()} at {errorIndex and varBinds[int(errorIndex) - 1][0] or "?"}')
        else:
            return int(varBinds[0][1])
    except Exception as e:
        print(f"Error in get_snmp_data: {e}")
        return None

def fetch_http_requests_total():
    try:
        http_requests_total = get_snmp_data(host, port, community_string, 'EXPERIMENTAL-MY-MIB::httpRequestsTotal.0')
        return http_requests_total, time.strftime('%Y-%m-%d %H:%M:%S')
    except Exception as e:
        print(f"Error in fetch_http_requests_total: {e}")
        return 0, time.strftime('%Y-%m-%d %H:%M:%S')

def fetch_http_requests_per_second():
    try:
        http_requests_per_second = get_snmp_data(host, port, community_string, 'EXPERIMENTAL-MY-MIB::httpRequestsPerSecond.0')
        return http_requests_per_second, time.strftime('%Y-%m-%d %H:%M:%S')
    except Exception as e:
        print(f"Error in fetch_http_requests_per_second: {e}")
        return 0, time.strftime('%Y-%m-%d %H:%M:%S')

def fetch_http_response_time():
    try:
        http_response_time = get_snmp_data(host, port, community_string, 'EXPERIMENTAL-MY-MIB::httpResponseTime.0')
        return http_response_time, time.strftime('%Y-%m-%d %H:%M:%S')
    except Exception as e:
        print(f"Error in fetch_http_response_time: {e}")
        return 0, time.strftime('%Y-%m-%d %H:%M:%S')

def fetch_http_active_connections():
    try:
        http_active_connections = get_snmp_data(host, port, community_string, 'EXPERIMENTAL-MY-MIB::httpActiveConnections.0')
        return http_active_connections, time.strftime('%Y-%m-%d %H:%M:%S')
    except Exception as e:
        print(f"Error in fetch_http_active_connections: {e}")
        return 0, time.strftime('%Y-%m-%d %H:%M:%S')

X_requests_total = deque(maxlen=20)
Y_requests_total = deque(maxlen=20)

X_requests_per_second = deque(maxlen=20)
Y_requests_per_second = deque(maxlen=20)

X_response_time = deque(maxlen=20)
Y_response_time = deque(maxlen=20)

X_active_connections = deque(maxlen=20)
Y_active_connections = deque(maxlen=20)

app = dash.Dash(__name__)

app.layout = html.Div([
    html.Div([
        dcc.Interval(
            id='graph-update',
            interval=5*1000,
            n_intervals=0
        ),
        dcc.Graph(id='live-graph-requests-total'),
        dcc.Graph(id='live-graph-requests-per-second'),
        dcc.Graph(id='live-graph-response-time'),
        dcc.Graph(id='live-graph-active-connections'),
    ])
])

@app.callback(Output('live-graph-requests-total', 'figure'),
              [Input('graph-update', 'n_intervals')])
def update_graph_requests_total(n):
    requests_total, current_time = fetch_http_requests_total()
    X_requests_total.append(current_time)
    Y_requests_total.append(requests_total)

    trace = go.Scatter(x=list(X_requests_total), y=list(Y_requests_total), mode='lines+markers', name='Total de Requisições HTTP')
    data = [trace]

    layout = go.Layout(title='Total de Requisições HTTP',
                       xaxis=dict(title='Tempo'),
                       yaxis=dict(title='Requisições'))

    return {'data': data, 'layout': layout}

@app.callback(Output('live-graph-requests-per-second', 'figure'),
              [Input('graph-update', 'n_intervals')])
def update_graph_requests_per_second(n):
    requests_per_second, current_time = fetch_http_requests_per_second()
    X_requests_per_second.append(current_time)
    Y_requests_per_second.append(requests_per_second)

    trace = go.Scatter(x=list(X_requests_per_second), y=list(Y_requests_per_second), mode='lines+markers', name='Requisições HTTP por Segundo')
    data = [trace]

    layout = go.Layout(title='Requisições HTTP por Segundo',
                       xaxis=dict(title='Tempo'),
                       yaxis=dict(title='Requisições/segundo'))

    return {'data': data, 'layout': layout}

@app.callback(Output('live-graph-response-time', 'figure'),
              [Input('graph-update', 'n_intervals')])
def update_graph_response_time(n):
    response_time, current_time = fetch_http_response_time()
    X_response_time.append(current_time)
    Y_response_time.append(response_time)

    trace = go.Scatter(x=list(X_response_time), y=list(Y_response_time), mode='lines+markers', name='Tempo de Resposta HTTP')
    data = [trace]

    layout = go.Layout(title='Tempo de Resposta HTTP',
                       xaxis=dict(title='Tempo'),
                       yaxis=dict(title='Milissegundos'))

    return {'data': data, 'layout': layout}

@app.callback(Output('live-graph-active-connections', 'figure'),
              [Input('graph-update', 'n_intervals')])
def update_graph_active_connections(n):
    active_connections, current_time = fetch_http_active_connections()
    X_active_connections.append(current_time)
    Y_active_connections.append(active_connections)

    trace = go.Scatter(x=list(X_active_connections), y=list(Y_active_connections), mode='lines+markers', name='Conexões HTTP Ativas')
    data = [trace]

    layout = go.Layout(title='Conexões HTTP Ativas',
                       xaxis=dict(title='Tempo'),
                       yaxis=dict(title='Conexões'))

    return {'data': data, 'layout': layout}

if __name__ == '__main__':
    app.run_server(host='0.0.0.0', port=8080, debug=True)