// md-watch: the running Machinemodule engine's live stats, read from its shared segment (/proc/<md-engine>/fd/3), once a second.
//   md-watch [seconds=30]   (as root on the Move)
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include "md_shm.h"

int main(int argc, char** argv)
{
	const int secs = argc > 1 ? std::atoi(argv[1]) : 30;
	std::string pid;
	if(DIR* d = opendir("/proc"))
	{
		while(dirent* e = readdir(d))
		{
			char comm[64] = {}; std::string p = std::string("/proc/") + e->d_name + "/comm";
			if(FILE* f = std::fopen(p.c_str(), "r")) { if(std::fgets(comm, sizeof comm, f) && std::string(comm) == "md-engine\n") pid = e->d_name; std::fclose(f); }
		}
		closedir(d);
	}
	if(pid.empty()) { std::fprintf(stderr, "no md-engine running\n"); return 1; }
	const int fd = open(("/proc/" + pid + "/fd/3").c_str(), O_RDONLY);
	auto* s = fd < 0 ? nullptr : static_cast<const mm::shm::Segment*>(mmap(nullptr, sizeof(mm::shm::Segment), PROT_READ, MAP_SHARED, fd, 0));
	if(!s || s == MAP_FAILED) { std::fprintf(stderr, "cannot map the segment\n"); return 1; }
	uint32_t u0 = s->underruns.load(), k0 = s->skips.load(), r0 = s->stats.releases.load();
	std::printf("LTNCY %u blocks  (per second: late blocks, skips, last/max block us, voice releases)\n", s->depth.load());
	for(int i = 0; i < secs; ++i)
	{
		sleep(1);
		const uint32_t u = s->underruns.load(), k = s->skips.load(), r = s->stats.releases.load();
		std::printf("%2d s: late %4u  skips %3u  block %5u us  max %6u us  releases %u\n", i + 1, u - u0, k - k0, s->stats.lastUs.load(), s->stats.maxUs.load(), r - r0);
		std::fflush(stdout);
		u0 = u; k0 = k; r0 = r;
	}
	return 0;
}
