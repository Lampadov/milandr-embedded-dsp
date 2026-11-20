#include <MDR32FxQI_port.h>
#include <MDR32FxQI_rst_clk.h>
#include <math.h>
#include "arm_math.h"

void Delay(int waitTicks);

#define freq 333
#define fs 1024
#define SAMPLE_BUFFER_LENGTH        1024
#define SAMPLE_BUFFER_LENGTH_HALF   (SAMPLE_BUFFER_LENGTH/2)

uint32_t check;
float fft_input[SAMPLE_BUFFER_LENGTH];
float fft_output[SAMPLE_BUFFER_LENGTH];
float fft_power[SAMPLE_BUFFER_LENGTH_HALF];
uint8_t     ifftFlag                = 0;


int main()
{
	arm_status status;

	MDR_RST_CLK -> PER_CLOCK |= (1 << 19) | (1 << 21);
	MDR_RST_CLK -> UART_CLOCK |= (1 << 26);
	MDR_TIMER4 -> ARR = 0xFFFFFFFF;


	/* HSE, 80 MHz */
  MDR_RST_CLK -> CPU_CLOCK 	= 0x00000006;
  MDR_RST_CLK -> HS_CONTROL 	= 0x00000001;
  while(!(MDR_RST_CLK ->CLOCK_STATUS &0x00000004));
  MDR_RST_CLK -> PLL_CONTROL	= 0x00000904;
  while(!(MDR_RST_CLK ->CLOCK_STATUS&0x00000002));
  MDR_RST_CLK -> CPU_CLOCK	= 0x00000106;

	/* Write signal to array */
	for (int i = 0; i < SAMPLE_BUFFER_LENGTH; i++) {
			fft_input[i] = sin(2*3.1415926f*freq*i/fs);
	}

	/* Analyze signal */
	arm_rfft_fast_instance_f32 fft;
	status = arm_rfft_fast_init_f32(&fft, SAMPLE_BUFFER_LENGTH);
	MDR_TIMER4 -> CNTRL = 0x1;
	arm_rfft_fast_f32(&fft, fft_input, fft_output, ifftFlag);
	check = MDR_TIMER4 -> CNT;
	arm_cmplx_mag_f32(fft_output, fft_power, SAMPLE_BUFFER_LENGTH_HALF);

	/* Find dominant frequency */
	float32_t   maxValue;
	uint32_t    maxIndex;
	arm_max_f32(fft_power, SAMPLE_BUFFER_LENGTH_HALF, &maxValue, &maxIndex);

  while (1)
  {
  }
}


void Delay(int waitTicks)
{
  int i;
  for (i = 0; i < waitTicks; i++)
  {
    __NOP();
  }
}
