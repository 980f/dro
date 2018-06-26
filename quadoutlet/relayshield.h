#pragma once
//fragment for driving a seeedstudio relayshield v1.3
//4 relays on d0..d3, but fairly easy to cut and jumper to other pins
//a 1 is relay on

OutputPin<0> R1;
OutputPin<1> R2;
OutputPin<2> R3;
OutputPin<3> R4;

BoolishRef *relay[4]={
  &R1,&R2,&R3,&R4
};

