#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <elf.h>

#if CPUINFO_MOCK
	#include <cpuinfo-mock.h>
#endif

#include <cpuinfo.h>
#include <powerpc/linux/api.h>
#include <cpuinfo/log.h>

#include <sys/auxv.h>

#if CPUINFO_MOCK
	static uint32_t mock_hwcap = 0;
	void cpuinfo_set_hwcap(uint32_t hwcap) {
		mock_hwcap = hwcap;
	}
#endif

uint32_t cpuinfo_powerpc_linux_hwcap_from_getauxval(void)
{
	#if CPUINFO_MOCK
		return mock_hwcap;
	#else
		return (uint32_t) getauxval(AT_HWCAP);
	#endif
}
