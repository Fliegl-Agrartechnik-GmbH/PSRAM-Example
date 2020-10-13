/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"




// Profile fun! microseconds to seconds
double GetTime() { return (double)esp_timer_get_time() / 1000000; }

int RamTest()
	{
	int rs[] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4000 };
	printf("Ram Speed Test!\n\n");
	char xx = 0;
	for (int a = 0; a < 13; a++)
		{
		printf("Read Speed 8bit ArraySize %4dkb ", rs[a]);
		int ramsize = rs[a] * 1024;
		char * rm = (char*)malloc(ramsize);

		int iters = 10; // Just enuff to boot the dog
		if (rs[a] < 512) iters = 50;
		double st = GetTime();
		for (int b = 0; b < iters; b++)
			for (int c = 0; c < ramsize; c++)
				xx |= rm[c];
		st = GetTime() - st;
		vTaskDelay(1); // Dog it!
		double speed = ((double)(iters*ramsize ) / (1024 * 1024)) / (st);
		printf(" time: %2.1f %2.1f mb/sec  \n", st, speed);
		free(rm);
		}
	printf("\n");
	for (int a = 0; a < 13; a++)
		{
		printf("Read Speed 16bit ArraySize %4dkb ", rs[a]);
		int ramsize = rs[a] * 1024;
		short * rm = (short*)malloc(ramsize);

		int iters = 10; // Just enuff to boot the dog
		if (rs[a] < 512) iters = 50;
		double st = GetTime();
		for (int b = 0; b < iters; b++)
			for (int c = 0; c < ramsize/2; c++)
				xx |= rm[c];
		st = GetTime() - st;
		vTaskDelay(1); // Dog it!
		double speed = ((double)(iters*ramsize) / (1024 * 1024)) / (st);
		printf(" time: %2.1f %2.1f mb/sec  \n", st, speed);
		free(rm);
		}
	printf("\n");
	for (int a = 0; a < 13; a++)
		{
		printf("Read Speed 32bit ArraySize %4dkb ", rs[a]);
		int ramsize = rs[a] * 1024;
		int * rm = (int*)malloc(ramsize);

		int iters = 10; // Just enuff to boot the dog
		if (rs[a] < 512) iters = 50;
		double st = GetTime();
		for (int b = 0; b < iters; b++)
			for (int c = 0; c < ramsize/4; c++)
				xx |= rm[c];
		st = GetTime() - st;
		vTaskDelay(1); // Dog it!
		double speed = ((double)(iters*ramsize) / (1024 * 1024)) / (st);
		printf(" time: %2.1f %2.1f mb/sec  \n", st, speed);
		free(rm);
		}
	printf("Test done!\n");
	return xx;
	}

void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Free heap: %d\n", esp_get_free_heap_size());

    RamTest();

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    RamTest();
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
