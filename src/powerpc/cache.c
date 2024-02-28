#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <cpuinfo.h>
#include <cpuinfo/log.h>
#include <powerpc/api.h>

#define NUM_CACHE   4
#define BUF_SIZE    128

int path_exist(const char *path)
{
	return (access(path, F_OK) == 0);
}

void read_str(char *result, size_t len, const char *path)
{
	FILE *f;

	f = fopen(path, "r");

	fgets(result, len, f);
	fclose(f);

	len = strlen(result);
	if (result[len - 1] == '\n')
		result[len - 1] = '\0';
}

void cpuinfo_powerpc_decode_cache(
	uint32_t smt_id,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1])
{
	char buf[BUF_SIZE];
	char result[BUF_SIZE];
	uint32_t i;
	uint32_t size = 0, associativity = 0, line_size = 0, sets = 0;
	size_t len;

	for(i = 0; i < NUM_CACHE; i++) {
		sprintf (buf, "/sys/devices/system/cpu/cpu%d/cache/index%d/size", smt_id, i);

		if(path_exist(buf))
			read_str(result, sizeof(result), buf);

		len = strlen(result);

		if (result[len - 1] == 'K')
			result[len - 1] = '\0';

		size = atoi(result);
		sprintf (buf, "/sys/devices/system/cpu/cpu%d/cache/index%d/ways_of_associativity", smt_id, i);

		if(path_exist(buf))
			read_str(result, sizeof(result), buf);

		associativity = atoi(result);
		sprintf (buf, "/sys/devices/system/cpu/cpu%d/cache/index%d/coherency_line_size", smt_id, i);

		if(path_exist(buf))
		read_str(result, sizeof(result), buf);

		line_size = atoi(result);

		sprintf (buf, "/sys/devices/system/cpu/cpu%d/cache/index%d/number_of_sets", smt_id, i);
		if(path_exist(buf))
			read_str(result, sizeof(result), buf);

		sets = atoi(result);

		switch (i) {
			case 0:
				*l1d = (struct cpuinfo_cache) {
						.size = size * 1024,
						.associativity = associativity,
						.line_size = line_size,
						.sets = sets
				};
				break;
			case 1:
				*l1i = (struct cpuinfo_cache) {
						.size = size * 1024,
						.associativity = associativity,
						.line_size = line_size,
						.sets = sets
				};
				break;
			case 2:
				*l2 = (struct cpuinfo_cache) {
						.size = size * 1024,
						.associativity = associativity,
						.line_size = line_size,
						.sets = sets
				};
				break;
			case 3:
				*l3 = (struct cpuinfo_cache) {
						.size = size * 1024,
						.associativity = associativity,
						.line_size = line_size,
						.sets = sets
				};
				break;

		}
	}
}
