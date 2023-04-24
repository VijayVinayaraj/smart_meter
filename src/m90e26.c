#include "m90e26.h"
 spi_device_handle_t m90e20;
 static const char TAG[]="spi";


void m90e26WriteU16( uint8_t address, uint16_t val) {
	
	spi_transaction_t m90e26_buf ;
	memset(&m90e26_buf, 0, sizeof(m90e26_buf));
	m90e26_buf.length		= 8 * 3;
	m90e26_buf.flags 		= SPI_TRANS_USE_TXDATA ;
	m90e26_buf.tx_data[0]	= address ;
	m90e26_buf.tx_data[1]	= val >> 8 ;
	m90e26_buf.tx_data[2]	= val & 0xFF ;
	
	ESP_ERROR_CHECK(spi_device_transmit(m90e20 ,&m90e26_buf)) ;
	
	ESP_LOGI(TAG, "TX: addr=%02x d0=%02x d1=%02x\r\n", m90e26_buf.tx_data[0], m90e26_buf.tx_data[1], m90e26_buf.tx_data[2]) ;
}


uint16_t m90e26ReadU16(uint8_t address) {
	
	spi_transaction_t m90e26_buf ;
	memset(&m90e26_buf, 0, sizeof(m90e26_buf)) ;
	m90e26_buf.length		= 8 * 3 ;
	m90e26_buf.flags 		= SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA ;
	m90e26_buf.tx_data[0]	= address | 0x80 ;
	
	ESP_ERROR_CHECK(spi_device_transmit(m90e20, &m90e26_buf)) ;
	
	ESP_LOGI(TAG, "RX: addr=%02x  d0=%02x  d1=%02x  dx=%04x\r\n", m90e26_buf.tx_data[0], m90e26_buf.rx_data[1], m90e26_buf.rx_data[2], (m90e26_buf.rx_data[1] << 8) | m90e26_buf.rx_data[2]) ;
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
    //     .clock_speed_hz =200000,
    //     .mode =3,
    //     .spics_io_num=VSPI_CS1,
    //     .queue_size=3,
    //    .address_bits = 8,
    //     .duty_cycle_pos		= 128
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


