void delay_us(unsigned int dlyUS)
{
   //for(dlyUS*=12;dlyUs>0;dlyUS--);
   dlyUS*=12;
   while(dlyUS--);
}
void delay_ms(unsigned int dlyMS)
{
     //for(dlyMS*=12000;dlyMS>0;dlyMS--);
	 dlyMS*=12000;
	 while(dlyMS--);
}
void delay_s(unsigned int dlyS)
{
      //for(dlyS*=12000000;dlyS>0;dlyS--);
	  dlyS*=12000000;
	  while(dlyS--);
}
