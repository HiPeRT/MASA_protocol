# masa_protocol

Client-Server protocol, tailored to send different tipes of datagrams via UDP or TCP.

## Usage
TODO

---
### How to add a new datagram type

  1. In **include/objects.hpp** define your data structure, if necessary.
  2. In **include/messages.hpp** define a new message structure. Each message should be child of ```struct Message```.
  3. Since each different datagram is intended to be sent to a different port, please note the file ``protocol_ports.md``