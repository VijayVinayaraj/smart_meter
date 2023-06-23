#include "m90e26.h"
 spi_device_handle_t m90e20;
 static const char TAG[]="spi";

  uint16_t metering[11];
  enum metering_values {
    _plconsth,
    _plconstl,
    _lgain,
    _lphi,
    _ngain,
    _nphi,
    _pstartth,
    _pnolth,
    _qstartth,
    _qnolth,
    _mmode
  };
  //uint16_t _crc1;
  uint16_t measurement[10];
  enum measurement_values {
    _ugain,
    _igain,
    _igainn,
    _uoffset,
    _ioffestl,
    _ioffsetn,
    _poffestl,
    _qoffsetl,
    _poffsetn,
    _qoffsetn
  };



void m90e26WriteU16( uint8_t address, uint16_t val) {
	
	spi_transaction_t m90e26_buf ;
	memset(&m90e26_buf, 0, sizeof(m90e26_buf));
	m90e26_buf.length		= 8 * 3;
	m90e26_buf.flags 		= SPI_TRANS_USE_TXDATA ;
	m90e26_buf.tx_data[0]	= address ;
	m90e26_buf.tx_data[1]	= val >> 8 ;
	m90e26_buf.tx_data[2]	= val & 0xFF ;
	
	ESP_ERROR_CHECK(spi_device_transmit(m90e20 ,&m90e26_buf)) ;
	
	//ESP_LOGI(TAG, "TX: addr=%02x d0=%02x d1=%02x\r\n", m90e26_buf.tx_data[0], m90e26_buf.tx_data[1], m90e26_buf.tx_data[2]) ;
}


uint16_t m90e26ReadU16(uint8_t address) {
	
	spi_transaction_t m90e26_buf ;
	memset(&m90e26_buf, 0, sizeof(m90e26_buf)) ;
	m90e26_buf.length		= 8 * 3 ;
	m90e26_buf.flags 		= SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA ;
	m90e26_buf.tx_data[0]	= address | 0x80 ;
	
	ESP_ERROR_CHECK(spi_device_transmit(m90e20, &m90e26_buf)) ;
	
	//ESP_LOGI(TAG, "RX: addr=%02x  d0=%02x  d1=%02x  dx=%04x\r\n", m90e26_buf.tx_data[0], m90e26_buf.rx_data[1], m90e26_buf.rx_data[2], (m90e26_buf.rx_data[1] << 8) | m90e26_buf.rx_data[2]) ;
	return (m90e26_buf.rx_data[1] << 8) | m90e26_buf.rx_data[2] ;
}
void spi_init(){

    

    spi_bus_config_t buscfg={
        .mosi_io_num=VSPI_MOSI,
        .miso_io_num=VSPI_MISO,
        .sclk_io_num=VSPI_SCLK,
        .quadhd_io_num =-1,
        .quadwp_io_num =-1,
        .max_transfer_sz = (4 * 8)
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST,&buscfg,1));
    spi_device_interface_config_t devcfg={
    .command_bits		= 0,
		.address_bits		= 0,
		.dummy_bits			= 0,
		.mode				= 3,						// only SPI mode 3 supported
		.duty_cycle_pos		= 128,						// same as 0 = 50/50% duty cycle
		.cs_ena_pretrans 	= 0,
		.cs_ena_posttrans	= 0,
		.clock_speed_hz		= 160000,
		.input_delay_ns		= 0,
		.spics_io_num		= VSPI_CS1,
		.flags				= 0,
		.queue_size			= 16,
		.pre_cb				= 0,						// no callback handler
		.post_cb			= 0,
    };
    
    esp_err_t ret=    spi_bus_add_device(SPI3_HOST, &devcfg, &m90e20);
   if(ret ==ESP_OK)
    ESP_LOGI(TAG,"added device");
    else
    ESP_LOGI(TAG,"not added device");

    m90e26WriteU16(SOFTRESET,CODE_RESET);

}


uint16_t getSystemStatus(){
    m90e26ReadU16(0x31);
    m90e26ReadU16(0x32);
    m90e26ReadU16(0x33);
    return m90e26ReadU16(SYS_STATUS);
}


uint16_t getMeterStatus(){
    return m90e26ReadU16(MET_STATUS);
}

double getLineVoltage(){
    uint16_t voltage = m90e26ReadU16(V_RMS);
    return (double)voltage/100;
}

double getLineCurrent(){
    uint16_t current = m90e26ReadU16(I_RMS_L);
    return (double)current/1000;
}

double getActivePower(){
    int16_t activePower = (int16_t) m90e26ReadU16(P_ACT_L);
    return (double)activePower;
}

double getPowerFactor (){
    int16_t powerFactor = (int16_t) m90e26ReadU16(P_FACTOR_L);
    if (powerFactor & 0x8000) {
    powerFactor = (powerFactor & 0x7FFF) * -1;
  }
  return (double)powerFactor / 1000;

}

uint16_t checksumCalc(uint8_t id){
    metering[_plconsth] = 0x00B9;
  metering[_plconstl] = 0xC1F3;
  metering[_lgain] = 0x1D39;
  metering[_lphi] = 0x0000;
  metering[_ngain] = 0x0000;
  metering[_nphi] = 0x0000;
  metering[_pstartth] = 0x08BD;
  metering[_pnolth] = 0x0000;
  metering[_qstartth] = 0x0AEC;
  metering[_qnolth] = 0x0000;
  metering[_mmode] = 0x9422;
  //_crc1 = 0x4A34;

  measurement[_ugain] = 0x37E8;
  measurement[_igain] = 0x11DA;
  measurement[_igainn] = 0x7530;
  measurement[_uoffset] = 0x0000;
  measurement[_ioffestl] = 0x0000;
  measurement[_ioffsetn] = 0x0000;
  measurement[_poffestl] = 0x0000;
  measurement[_qoffsetl] = 0x0000;
  measurement[_poffsetn] = 0x0000;
  measurement[_qoffsetn] = 0x0000;
  //_crc2 = 0xD294;
    if(id == 1){
        uint8_t l2c = 0;
        uint8_t h2c = 0; 
        for (int i = 0; i < 11; i++) {
      l2c += metering[i];
      l2c += metering[i] >> 8;
      h2c ^= metering[i];
      h2c ^= metering[i] >> 8;
    }
    return (( uint16_t)h2c << 8) | l2c;
    }
     else if (id == 2) {
    uint8_t l3b = 0;
    uint8_t h3b = 0;
    for (int i = 0; i < 10; i++) {
      l3b += measurement[i];
      l3b += measurement[i] >> 8;
      h3b ^= measurement[i];
      h3b ^= measurement[i] >> 8;
    }
    return ((uint16_t)h3b << 8) | l3b;
  }
  return 0;
}

double getEnergy(){
  uint16_t energy =  m90e26ReadU16(E_ACT_FWD); // register is cleared after reading
  return (double)energy;
}

double getEnergyRev(){
  uint16_t energy =  m90e26ReadU16(E_ACT_REV);
  return (double)energy;
}



void calibrateIC(){

 

    
    // // start adjustments
    m90e26WriteU16(ADJSTART,CODE_START);
    m90e26WriteU16(U_GAIN,0x37E8); // gain for rms voltage
    m90e26WriteU16(I_GAIN_L,0x11DA);// gain for the rms current
     m90e26WriteU16(CRC_2,checksumCalc(2));


    // after calibration start metering
    m90e26WriteU16(CALSTART,CODE_CHECK);
    m90e26WriteU16(ADJSTART,CODE_CHECK);

   uint16_t systemstatus = getSystemStatus();

  if (systemstatus & 0xC000) {
    // checksum 1 error
    ESP_LOGE(TAG,"Checksum 1 Error!!");
  }
  if (systemstatus & 0x3000) {
    // checksum 2 error
    ESP_LOGE(TAG,"Checksum 2 Error!!");
  }
}
