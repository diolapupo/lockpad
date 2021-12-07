unsigned char read_eeprom(unsigned char addr){
    while(EECON1bits.RD || EECON1bits.WR);
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD=1;
    return EEDATA;
}
void write_eeprom(unsigned addr, unsigned char data){
    int gie_status;
    while(EECON1bits.WR);
    EEADR = addr;
    EEDATA = data;
    EECON1bits.EEPGD=0;
    WREN=1;
    gie_status = GIE;
    GIE=0;
    EECON2=0x55;
    EECON2=0xAA;
    EECON1bits.WR = 1;
    GIE = gie_status;
    WREN=0;
}