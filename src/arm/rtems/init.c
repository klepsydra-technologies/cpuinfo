#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

struct cpuinfo_arm_isa cpuinfo_isa = {
#if CPUINFO_ARCH_ARM
	.thumb = true,
	.armv7 = true,
	.vfpv2 = true,
	.vfpv3 = true,
	.neon = true,
#endif
#if CPUINFO_ARCH_ARM64
	.atomics = true,
	.bf16 = true,
	.sve = true,
	.sve2 = true,
	.i8mm = true,
#endif
};

void cpuinfo_arm_rtems_init(void) {
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;

  /* Allocate memory */
	processors = calloc(CONFIGURE_MAXIMUM_PROCESSORS, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("Failed to allocate for cpuinfo_processor");
		goto cleanup;
	}
	cores = calloc(CONFIGURE_MAXIMUM_PROCESSORS, sizeof(struct cpuinfo_core));
	if (cores == NULL) {
		cpuinfo_log_error("Failed to allocate for cpuinfo_core");
		goto cleanup;
	}

  /* Populate Data */
	/**
	 * TODO:
	 * Populate other structs for RTEMS Zed Board.
	 * Currently these are the only required information for QNNPACK.
	 */
    for (int i = 0; i < CONFIGURE_MAXIMUM_PROCESSORS; i++) {
      processors[i].core = cores;
      cores[i].uarch = cpuinfo_uarch_cortex_a9;
    }

  /* Commit */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
    cpuinfo_cores_count = CONFIGURE_MAXIMUM_PROCESSORS;

	__sync_synchronize();
	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;

cleanup:
	free(processors);
	free(cores);
}
