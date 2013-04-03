
void GPS_setCommand(uint8_t* command, uint8_t length) {
  boolean success=false;
  while(!success)
  {
    sendUBX(command, length);
    success=getUBX_ACK(command);
  }
}


// Send a byte array of UBX protocol to the GPS
void sendUBX(uint8_t *MSG, uint8_t len) {
  for(int i=0; i<len; i++) {
    GPS.write(MSG[i]);
    
#ifdef DEBUG_ON
    Serial.print(MSG[i], HEX);
#endif

  }
  
  GPS.println();
}


// Calculate expected UBX ACK packet and parse UBX response from GPS
boolean getUBX_ACK(uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();

#ifdef DEBUG_ON
  Serial.print(" ACK: ");
#endif

  // Construct the expected ACK packet    
  ackPacket[0] = 0xB5;	// header
  ackPacket[1] = 0x62;	// header
  ackPacket[2] = 0x05;	// class
  ackPacket[3] = 0x01;	// id
  ackPacket[4] = 0x02;	// length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];	// ACK class
  ackPacket[7] = MSG[3];	// ACK id
  ackPacket[8] = 0;		// CK_A
  ackPacket[9] = 0;		// CK_B

  // Calculate the checksums
  for (uint8_t i=2; i<8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }

  while (1) {

    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
#ifdef DEBUG_ON
      Serial.println(" Success");
#endif
      return true;
    }

    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) { 
#ifdef DEBUG_ON
      Serial.println(" Error");
#endif
      return false;
    }

    // Make sure data is available to read
    if (GPS.available()) {
      b = GPS.read();

      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) { 
        ackByteID++;
#ifdef DEBUG_ON
        Serial.print(b, HEX);
#endif
      } 
      else {
        ackByteID = 0;	// Reset and look again, invalid order
      }

    }
  }
}



