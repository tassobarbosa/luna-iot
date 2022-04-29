## Intro

The set of interactions between Client-Server and the initialization process are described here.

From this point on, each scenario describes how it works and publishes its package diagram.


## Init

Initialization consists of the following sequence:
Server:
- Server initialization process with possibility of multiple connections
- Create pool of authenticated connections
- Wait for authorization handshake with client
- Provides functions


Client:
- Tcp socket initialization process
- Asks for authentication to release functions

## Exchange packets


DIAGRAM PACKETS - SCENARIO [Authentication]
```
 CLIENT     SERVER     HARDWARE
 |          |          |
 |--------->|          |  {"addr":"127.0.0.1","command":"0xB0","data":"1","header":"0xA0","port":48792}
 |<---------|          |  {"addr":"127.0.0.1","command":"0xB1","data":"1","header":"0xA0","port":48792}
 |--------->|          |  {"addr":"127.0.0.1","command":"0xB2","data":"1","header":"0xA0","port":48792}
 |<---------|          |  {"addr":"127.0.0.1","command":"0xB3","data":"1","header":"0xA0","port":48792}
 |          |          |  

```

DIAGRAM PACKETS - SCENARIO [Request hardware information]
```
 CLIENT     SERVER     HARDWARE
 |          |          |
 |--------->|          |  {"addr":"127.0.0.1","command":"0xC4","data":"1","header":"0xA1","port":48794}
 |          |--------->|  READ SENSOR VALUE
 |          |<---------|  RETRIEVE OWN VALUE
 |<---------|          |  {"addr":"127.0.0.1","command":"0xC4","data":"27/04/22 20:43:17","header":"0xA1","port":48794}
 |          |          |  

```

DIAGRAM PACKETS - SCENARIO [Control hardware]
```
 CLIENT     SERVER     HARDWARE
 |          |          |
 |--------->|          |  {"addr":"127.0.0.1","command":"0xC1","data":"on","header":"0xA2","port":48794}
 |          |--------->|  WRITE VALUE TO SENSOR
 |          |          |  
 
 ```

 DIAGRAM PACKETS - SCENARIO [Polling]
```
 CLIENT     SERVER     HARDWARE
 |          |          |
 |--------->|          |  {"addr":"127.0.0.1","command":"0xD1","data":"1","header":"0xA3","port":48794}
 |<---------|          |  {"addr":"127.0.0.1","command":"0xD2","data":"1","header":"0xA3","port":48794}
 |          |          |
 
 ```