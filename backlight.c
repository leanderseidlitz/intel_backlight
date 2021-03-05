#include <stdio.h>
#include <stdlib.h>


unsigned long convert_ul(char *buf){
    char* endptr;
    unsigned long res = strtoull(buf, &endptr, 10);
    if (endptr == buf){
	exit(-1);
    }
    return res;
}

unsigned long read_file_to_ul(char* name) {
    char buf[16] = {0};
    FILE *file = fopen(name, "r");
    if (file == NULL){
	printf("Error opening %s, setuid missing?", name);
	exit(-1);
    }
    if (!fgets(buf, sizeof(buf)-1, file)){
	exit(-1);
    }
    fclose(file);
    return convert_ul(buf);
}

void write_ul_to_file(char* name, unsigned long val) {
    FILE *file = fopen(name, "wb");
    if (file == NULL){
	printf("Error opening %s\n", name);
	exit(-1);
    }
    fprintf(file, "%lu", val);
    fclose(file);
}

int main(int argc, char *argv[]){
    if (argc != 2){
	printf("usage: %s [+/-]<brightness>.\n", argv[0]);
	exit(0);
    }
    
    unsigned long max_brightness = read_file_to_ul("/sys/class/backlight/intel_backlight/max_brightness");
    unsigned long target_brightness = read_file_to_ul("/sys/class/backlight/intel_backlight/brightness");

    switch (argv[1][0]){
    case '+':
    {
	unsigned long diff = convert_ul(argv[1]+1);
	target_brightness = diff + target_brightness > max_brightness ? max_brightness : diff + target_brightness;
	break;
    }
    case '-':
    {
	unsigned long diff = convert_ul(argv[1]+1);
        target_brightness = target_brightness < diff ? 0 : target_brightness - diff;
	break;
    }
    default:
	target_brightness = convert_ul(argv[1]);
	break;
    }

    write_ul_to_file("/sys/class/backlight/intel_backlight/brightness", target_brightness);
}
