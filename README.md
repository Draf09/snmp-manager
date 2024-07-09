# snmp-manager

Trabalho 2 - Descrição
O trabalho consiste em escolher um elemento de rede para gerenciamento, descrever uma MIB para este elemento e implementar um agente para coleta de dados do elemento.

1. Escolha do elemento de rede

O elemento pode ser real ou simulado.

2. Escrita da MIB

A MIB deve ser escrita usando SMIv2 e deve conter de 10 a 15 informações sobre o elemento gerenciado com no mínimo 1 tabela. Obrigatoriamente devem ser implementados novos objetos, não sendo permitido o uso apenas de objetos já existentes na MIB-II ou em outras MIBs. Devem ser incluídos objetos com acesso para somente leitura e leitura/escrita.

3. Implementação do agente

O agente pode ser implementado usando o pacote net-snmp (http://www.net-snmp.org/) ou outra biblioteca/framework para desenvolvimento de agentes SNMP.

Net-snmp manpage (http://www.net-snmp.org/docs/man/snmpd.conf.html) - seção "EXTENDING AGENT FUNCTIONALITY"

O agente deverá permitir a execução de operações get, getnext e set.

Itens a serem entregues:

código fonte da implementação do agente

arquivo(s) da MIB do elemento gerenciado

artigo (em pdf) descrevendo o elemento gerenciado, a descrição dos objetos da MIB, detalhes de implementação do agente e exemplos de funcionamento do agente, através da execução de comandos SNMP


Fim descrição
-------------------------------------------------

O formato de arquivo para MIBs usando SMIv2 (Structure of Management Information Version 2) é definido pelo padrão ASN.1 (Abstract Syntax Notation One). Este padrão é utilizado para descrever a estrutura de dados de forma precisa e independente de máquina, o que é essencial para a interoperabilidade entre diferentes sistemas e dispositivos em uma rede gerenciada via SNMP (Simple Network Management Protocol).

Estrutura Básica de uma MIB SMIv2:
Cabeçalho do Módulo:
Inclui declarações de importação de tipos e macros de outros módulos SNMP, como SNMPv2-SMI e SNMPv2-TC, que são necessários para definir tipos de dados e convenções textuais.
Declaração MODULE-IDENTITY:
Define a identidade do módulo MIB, incluindo informações como a última atualização, organização, informações de contato e uma descrição do módulo. Esta declaração também inclui uma lista de revisões que documentam as alterações históricas do módulo 24.
Definições de Objectos:
Cada objeto gerenciado é descrito usando a macro OBJECT-TYPE, que inclui detalhes como o tipo de dado (por exemplo, INTEGER, OCTET STRING), a máxima permissão de acesso (por exemplo, read-only, read-write), e uma descrição textual.
Identificadores de Objeto:
Os objetos são organizados hierarquicamente e identificados por OIDs (Object Identifiers). Cada objeto ou grupo de objetos é atribuído a um identificador único dentro da árvore OID global.
Exemplo de Definição de MIB SMIv2:

Install directory: /var/lib/snmp/mibs
adduser: Warning: The home directory `/var/lib/snmp' does not belong to the user you are currently creating.
Created symlink /etc/systemd/system/multi-user.target.wants/snmpd.service → /lib/systemd/system/snmpd.service.
Setting up snmp-mibs-downloader (1.5) ...

