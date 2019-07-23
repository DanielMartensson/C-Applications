
#ifndef ACTIONS_H
#define ACTIONS_H

void talkToPeripheral(bd_addr connect_addr, int addressType, int minConnVal, int maxConnVal, int timeOut, int latency);
void talkToMaster();
int control();
void getInfo();
void scanning();
int read_message(int timeout_ms);

#endif
