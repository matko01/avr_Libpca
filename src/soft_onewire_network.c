#include <soft_onewire_network.h>

// undefine this to save some bytes
#define DESIGN_BY_CONTRACT 1


/* uint8_t ow_net_search_rom(soft_ow *a_bus, ow_romcode_t *a_romcodes, uint8_t max_devices) { */
/*  */
/* 	uint8_t id_bit_no = 0x00; */
/* 	uint8_t dev = 0x00; */
/* 	 */
/* 	#<{(| B | A |)}># */
/* 	uint8_t bits = 0x00; */
/* 	uint8_t search_direction = 0x00; */
/*  */
/* #ifdef DESIGN_BY_CONTRACT	 */
/* 	// input parameters validation */
/* 	if (!a_romcodes || !max_devices) { */
/* 		return 0; */
/* 	} */
/*  */
/* 	// single drop configuration */
/* 	if (!(a_bus & 0x02)) { */
/* 		max_devices = 1; */
/* 	} */
/* #endif */
/*  */
/* 	memset(a_romcodes, 0x00, max_devices * sizeof(ow_romcode_t)); */
/*  */
/* 	// no devices on the bus */
/* 	if (!sow_reset(a_bus)) { */
/* 		return 0; */
/* 	} */
/*  */
/* 	sow_write_byte(a_bus, OWN_SEARCH_ROM); */
/*  */
/* 	bits = _sow_read_bit(a_bus); // bit A */
/* 	bits |= (_sow_read_bit(a_bus) << 4); // bit B */
/*  */
/* 	if (0x11 == bits) { */
/* 		// two ones indicate no response from the devices (no one pulled the line low) */
/* 		// something is wrong with the physical layer */
/* 		return 0; */
/* 	} */
/* } */
/*  */

//--------------------------------------------------------------------------
// Find the 'first' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : no device present
//
int OWFirst()
{
   // reset the search state
   LastDiscrepancy = 0;
   LastDeviceFlag = FALSE;
   LastFamilyDiscrepancy = 0;

   return OWSearch();
}

//--------------------------------------------------------------------------
// Find the 'next' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
int OWNext()
{
   // leave the search state alone
   return OWSearch();
}

//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
int OWSearch()
{
	 SD | B | A
	----+---+---
     2  | 1 | 0
	uint8_t bits = 0x00;




   int id_bit_number;
   int last_zero, rom_byte_number, search_result;
   int id_bit, cmp_id_bit;
   unsigned char rom_byte_mask;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;
   crc8 = 0;

   // if the last call was not the last one
   if (!LastDeviceFlag)
   {
      // 1-Wire reset
      if (!OWReset())
      {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = FALSE;
         return FALSE;
      }

      // issue the search command 
      OWWriteByte(0xF0);  

      // loop to do the search
      do
      {
         // read a bit and its complement
		 bits = _sow_read_bit(a_bus); // bit A
		 bits |= (_sow_read_bit(a_bus) << 1); // bit B

         // check for no devices on 1-wire
         if (0x03 == bits)
            break;
         else
         {
            // all devices coupled have 0 or 1
			 if ((bits & 0x03)) {
				 bits &= ~0x04;
				 bits |= ((bits & 0x01) << 2);  // bit write value for search
			 }
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
				if (id_bit_number < LastDiscrepancy) {
				 bits &= ~0x04;

				}
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
				else {

				}
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (bits & 0x04)
               {
                  last_zero = id_bit_number;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (bits & 0x04)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            OWWriteBit((bits & 0x04));

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                docrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!((id_bit_number < 65) || (crc8 != 0)))
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0)
            LastDeviceFlag = TRUE;
         
         search_result = TRUE;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = FALSE;
      search_result = FALSE;
   }

   return search_result;
}
