 #include <REG51.H>
 #include <stdlib.h>
 #define stepper P2
 #define lcd_data_pin P1
          sbit EN = P3^2;// enable pin
          sbit RS = P3^0;//register select pin
          sbit RW = P3^1;// read write pin   
          sbit pushbutton = P3^4;
          sbit ledred=P3^5;
          sbit ledgreen=P3^3;
          sbit dip1=P3^6;
          sbit dip2=P3^7;
          sfr lcd_data_pin2=0xA0;  // data port P2

               unsigned int counter;
               unsigned int amount;
               unsigned int fuel_counter;
               unsigned int fuel;
               unsigned char c;
               unsigned int passenger_out;
               unsigned int seconds;
               unsigned int minutes;
               unsigned int hours;
               int num[10];							 
							 
							 void lcd_data2(unsigned char disp);    // function to send data on LCD
               void lcd_dataa(unsigned char *disp);    // function to send string to LCD
               void lcd_command(unsigned char comm); // function to send command to LCD
               void delay2(unsigned int msec);    //delay function
               void delay();
               void delay_lcd(int delay_time);// the time delay purposely for the lcd
               void lcd_cmd(unsigned char cmd_addr);   // Funtion to send command on LCD
               void lcd_data(unsigned int i);     //Function to send data on LCD

//main function
    void main(){
			        counter=0;
	            amount=90;
	            fuel_counter=0;
	            fuel=0;
	            passenger_out=0;	
	 
	        while(1){ 
						if(pushbutton==0){
							 if(dip1==0){
								    ledred=1;
								    ledgreen=0;
								    passenger_out=0;
                           }
													 else {
														    ledred=0;
														    ledgreen=1;
														    amount=90;
                                 }	        
				              stepper = 0x08;//1000****A
                      delay();
                      stepper = 0x0C;//1100****A,B
                      delay();
                      stepper = 0x04;//0100****B
                      delay();
                      stepper = 0x06;//0110****B,C
                      delay();
                      stepper = 0x02;//0010****C
                      delay();
                      stepper = 0x03;//0011****C,D
                      delay();
                      stepper = 0x01;//0001****D
                      delay();
                      stepper = 0x09;//1001****A,D
																 
	                    fuel_counter++;
	                               if(ledred==1){
																	    counter++;
																	    seconds=0x00;
																	    minutes=0x00;
																	    hours=0x00;
																	        
																	          if(counter==5){																							
																							        amount+=20;
                                                       //****start of the lcd interfacing*******//			
                                                      lcd_cmd(0x38);    // Configuring settings to 8-bit 2 row
																							        lcd_cmd(0x0E); 
																							        lcd_cmd(0x0C);    //Display on
																							        lcd_cmd(0x80);
																							        lcd_dataa("Amount:");//end of first amount
																							        lcd_cmd(0x88);    //Set cursor to blink at line 8 positon 1
																							        delay_lcd(250);
																							        lcd_data(amount);		
                                           //**********************end of lcd interfacing***************************//
																							         counter=0;
                                                            }
																														delay();
                                                  }
                                         }
																				 else{
																					    ledred=0;
																					    ledgreen=0;
																					    lcd_cmd(0x01);    //reset the lcd
   																					  amount=90;
																					    fuel=0;
    																					passenger_out=0;
																					    seconds=0;
																					    minutes=0;
    																					hours=0;
                                              }
															if(ledgreen==1){        //keeping track of the time when there is no passenger******
	                                    passenger_out+=1;
																        if(passenger_out%2==0){
																					         seconds+=1;
																					       if(seconds==60){
     																									minutes+=1;
																									    delay();
    																									seconds=0x00;
																									        if(minutes==60){
																														      hours+=1;
																														      delay();
																														      minutes=0x00;
																														         if(hours==24){
																																			        hours=0x00;
                                                                                  }
                                                                           }
                                                                  }
                                                              }
																												lcd_cmd(0x38);    // Configuring settings to 8-bit 2 row
																												lcd_cmd(0x0E); 
																												lcd_cmd(0x0C);    //Display on
																												lcd_cmd(0x90);
																												lcd_dataa("Empty:");//end of first amount
																												lcd_cmd(0x96);    //Set cursor to blink at line 1 positon 2
																												delay_lcd(250);
																												lcd_data(hours);
																												lcd_cmd(0x98);
																												lcd_dataa(":");//end of first amount
																												delay_lcd(250);
																												lcd_cmd(0x99);
																												delay_lcd(250);
																												lcd_data(minutes);
																												lcd_cmd(0x9B);
																												lcd_dataa(":");//end of first amount
																												lcd_cmd(0x9C);    //Set cursor to blink at line 1 positon 2
																												delay_lcd(250);
																												lcd_data(seconds);
                                             }//end of keeping track of the time when there is no passenger
												if(fuel_counter==10){//start of the fuel consumption count
													         fuel+=2000;
													         lcd_cmd(0x38);    // Configuring settings to 8-bit 2 row 
													         lcd_cmd(0x0E); 
													         lcd_cmd(0x0C);    //Display on
          												 lcd_cmd(0xC0);
													         lcd_dataa("Fuel:");//end of first amount
													         lcd_cmd(0xC8);    //Set cursor to blink at line 1 positon 3
													         delay_lcd(250);
													         lcd_data(fuel);
													         fuel_counter=0;
                                              }
                                      }
                                }
										void delay(){
											      unsigned char i,k,j;
											      for(i=0;i<2;i++)
             											for(j=0;j<100;j++)
											               for(k=0;k<100;k++);
                                  }
																	
										void delay_lcd(int delay_time)    // Time delay function for the lcd
																	 {
																		 int j,k;
																		 for(j=0;j<=delay_time;j++)
																		       for(k=0;k<=10;k++);
                                   }

                    void lcd_cmd(unsigned char cmd_addr)   // Funtion to send command on LCD
																	          {
																							lcd_data_pin = cmd_addr;
																							EN = 1;
																							RS = 0;
																							RW = 0;
																							delay_lcd(1);
																							EN = 0;
																							return;
                                            }

                 void lcd_data(unsigned int i)     //Function to send data on LCD
																						{   
																							int p;
																							int k=0;
																							while(i>0)
																								     {
																											 num[k]=i%10;
																											 i=i/10;
																											 k++;
                                                     }
																							k--;
																							for (p=k;p>=0;p--){
																								      c=num[p]+48;
																								      lcd_data_pin = c;
																								      RW = 0;
																								      RS = 1;
																								      EN = 1;
																								      delay_lcd(1);
																								      EN = 0;
                                          }
																					return;
                                   }
																	 
		    void delay2(unsigned int msec)    //delay function
																{
																	int i,j;
														for(i=0;i<msec;i++)
														for(j=0;j<100;j++);
                                }
																
        void lcd_data2(unsigned char disp)    // function to send data on LCD
																{
																	lcd_data_pin=disp;
																	EN=1;
																	RS=1;
																	RW=0;
																	delay2(1);
																	EN=0;
                                 }
																 
        void lcd_dataa(unsigned char *disp)    // function to send string to LCD
																 {
																	 int x;
																	 for(x=0;disp[x]!=0;x++)
																	 {
																		 lcd_data2(disp[x]);
                                   }
                                 }
