/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2014-2019 Intel Corporation
 */

#ifndef _INTEL_GUC_FWIF_H
#define _INTEL_GUC_FWIF_H

#include <linux/bits.h>
#include <linux/compiler.h>
#include <linux/types.h>
#include "gt/intel_engine_types.h"

#include "abi/guc_actions_abi.h"
#include "abi/guc_actions_pf_abi.h"
#include "abi/guc_actions_vf_abi.h"
#include "abi/guc_actions_slpc_abi.h"
#include "abi/guc_errors_abi.h"
#include "abi/guc_communication_mmio_abi.h"
#include "abi/guc_communication_ctb_abi.h"
#include "abi/guc_klvs_abi.h"
#include "abi/guc_messages_abi.h"

static inline const char *hxg_type_to_string(u32 type)
{
	switch (type) {
	case GUC_HXG_TYPE_REQUEST:
		return "request";
	case GUC_HXG_TYPE_EVENT:
		return "event";
	case GUC_HXG_TYPE_NO_RESPONSE_BUSY:
		return "busy";
	case GUC_HXG_TYPE_NO_RESPONSE_RETRY:
		return "retry";
	case GUC_HXG_TYPE_RESPONSE_FAILURE:
		return "failure";
	case GUC_HXG_TYPE_RESPONSE_SUCCESS:
		return "response";
	default:
		return "<invalid>";
	}
}

/* Payload length only i.e. don't include G2H header length */
#define G2H_LEN_DW_SCHED_CONTEXT_MODE_SET	2
#define G2H_LEN_DW_DEREGISTER_CONTEXT		1
#define G2H_LEN_DW_INVALIDATE_TLB		1

#define GUC_CONTEXT_DISABLE		0
#define GUC_CONTEXT_ENABLE		1

#define GUC_CLIENT_PRIORITY_KMD_HIGH	0
#define GUC_CLIENT_PRIORITY_HIGH	1
#define GUC_CLIENT_PRIORITY_KMD_NORMAL	2
#define GUC_CLIENT_PRIORITY_NORMAL	3
#define GUC_CLIENT_PRIORITY_NUM		4

#define GUC_MAX_CONTEXT_ID		65535
#define	GUC_INVALID_CONTEXT_ID		GUC_MAX_CONTEXT_ID

#define GUC_RENDER_ENGINE		0
#define GUC_VIDEO_ENGINE		1
#define GUC_BLITTER_ENGINE		2
#define GUC_VIDEOENHANCE_ENGINE		3
#define GUC_VIDEO_ENGINE2		4
#define GUC_MAX_ENGINES_NUM		(GUC_VIDEO_ENGINE2 + 1)

#define GUC_RENDER_CLASS		0
#define GUC_VIDEO_CLASS			1
#define GUC_VIDEOENHANCE_CLASS		2
#define GUC_BLITTER_CLASS		3
#define GUC_COMPUTE_CLASS		4
#define GUC_LAST_ENGINE_CLASS		GUC_COMPUTE_CLASS
#define GUC_MAX_ENGINE_CLASSES		16
#define GUC_MAX_INSTANCES_PER_CLASS	32

#define GUC_DOORBELL_INVALID		256

/*
 * Work queue item header definitions
 *
 * Work queue is circular buffer used to submit complex (multi-lrc) submissions
 * to the GuC. A work queue item is an entry in the circular buffer.
 */
#define WQ_STATUS_ACTIVE		1
#define WQ_STATUS_SUSPENDED		2
#define WQ_STATUS_CMD_ERROR		3
#define WQ_STATUS_ENGINE_ID_NOT_USED	4
#define WQ_STATUS_SUSPENDED_FROM_RESET	5
#define WQ_TYPE_BATCH_BUF		0x1
#define WQ_TYPE_PSEUDO			0x2
#define WQ_TYPE_INORDER			0x3
#define WQ_TYPE_NOOP			0x4
#define WQ_TYPE_MULTI_LRC		0x5
#define WQ_TYPE_MASK			GENMASK(7, 0)
#define WQ_LEN_MASK			GENMASK(26, 16)

#define WQ_GUC_ID_MASK			GENMASK(15, 0)
#define WQ_RING_TAIL_MASK		GENMASK(28, 18)

#define GUC_STAGE_DESC_ATTR_ACTIVE	BIT(0)
#define GUC_STAGE_DESC_ATTR_PENDING_DB	BIT(1)
#define GUC_STAGE_DESC_ATTR_KERNEL	BIT(2)
#define GUC_STAGE_DESC_ATTR_PREEMPT	BIT(3)
#define GUC_STAGE_DESC_ATTR_RESET	BIT(4)
#define GUC_STAGE_DESC_ATTR_WQLOCKED	BIT(5)
#define GUC_STAGE_DESC_ATTR_PCH		BIT(6)
#define GUC_STAGE_DESC_ATTR_TERMINATED	BIT(7)

#define GUC_CTL_LOG_PARAMS		0
#define   GUC_LOG_VALID			BIT(0)
#define   GUC_LOG_NOTIFY_ON_HALF_FULL	BIT(1)
#define   GUC_LOG_CAPTURE_ALLOC_UNITS	BIT(2)
#define   GUC_LOG_LOG_ALLOC_UNITS	BIT(3)
#define   GUC_LOG_CRASH_SHIFT		4
#define   GUC_LOG_CRASH_MASK		(0x3 << GUC_LOG_CRASH_SHIFT)
#define   GUC_LOG_DEBUG_SHIFT		6
#define   GUC_LOG_DEBUG_MASK	        (0xF << GUC_LOG_DEBUG_SHIFT)
#define   GUC_LOG_CAPTURE_SHIFT		10
#define   GUC_LOG_CAPTURE_MASK	        (0x3 << GUC_LOG_CAPTURE_SHIFT)
#define   GUC_LOG_BUF_ADDR_SHIFT	12

#define GUC_CTL_WA			1
#define   GUC_WA_POLLCS                 BIT(18)

#define GUC_CTL_FEATURE			2
#define   GUC_CTL_ENABLE_SLPC		BIT(2)
#define   GUC_CTL_DISABLE_SCHEDULER	BIT(14)

#define GUC_CTL_DEBUG			3
#define   GUC_LOG_VERBOSITY_SHIFT	0
#define   GUC_LOG_VERBOSITY_LOW		(0 << GUC_LOG_VERBOSITY_SHIFT)
#define   GUC_LOG_VERBOSITY_MED		(1 << GUC_LOG_VERBOSITY_SHIFT)
#define   GUC_LOG_VERBOSITY_HIGH	(2 << GUC_LOG_VERBOSITY_SHIFT)
#define   GUC_LOG_VERBOSITY_ULTRA	(3 << GUC_LOG_VERBOSITY_SHIFT)
/* Verbosity range-check limits, without the shift */
#define	  GUC_LOG_VERBOSITY_MIN		0
#define	  GUC_LOG_VERBOSITY_MAX		3
#define	  GUC_LOG_VERBOSITY_MASK	0x0000000f
#define	  GUC_LOG_DESTINATION_MASK	(3 << 4)
#define   GUC_LOG_DISABLED		(1 << 6)
#define   GUC_PROFILE_ENABLED		(1 << 7)

#define GUC_CTL_ADS			4
#define   GUC_ADS_ADDR_SHIFT		1
#define   GUC_ADS_ADDR_MASK		(0xFFFFF << GUC_ADS_ADDR_SHIFT)

#define GUC_CTL_DEVID			5

#define GUC_CTL_MAX_DWORDS		(SOFT_SCRATCH_COUNT - 2) /* [1..14] */

/* Generic GT SysInfo data types */
#define GUC_GENERIC_GT_SYSINFO_SLICE_ENABLED		0
#define GUC_GENERIC_GT_SYSINFO_VDBOX_SFC_SUPPORT_MASK	1
#define GUC_GENERIC_GT_SYSINFO_DOORBELL_COUNT_PER_SQIDI	2
#define GUC_GENERIC_GT_SYSINFO_MAX			16

/*
 * The class goes in bits [0..2] of the GuC ID, the instance in bits [3..6].
 * Bit 7 can be used for operations that apply to all engine classes&instances.
 */
#define GUC_ENGINE_CLASS_SHIFT		0
#define GUC_ENGINE_CLASS_MASK		(0x7 << GUC_ENGINE_CLASS_SHIFT)
#define GUC_ENGINE_INSTANCE_SHIFT	3
#define GUC_ENGINE_INSTANCE_MASK	(0xf << GUC_ENGINE_INSTANCE_SHIFT)
#define GUC_ENGINE_ALL_INSTANCES	BIT(7)

#define MAKE_GUC_ID(class, instance) \
	(((class) << GUC_ENGINE_CLASS_SHIFT) | \
	 ((instance) << GUC_ENGINE_INSTANCE_SHIFT))

#define GUC_ID_TO_ENGINE_CLASS(guc_id) \
	(((guc_id) & GUC_ENGINE_CLASS_MASK) >> GUC_ENGINE_CLASS_SHIFT)
#define GUC_ID_TO_ENGINE_INSTANCE(guc_id) \
	(((guc_id) & GUC_ENGINE_INSTANCE_MASK) >> GUC_ENGINE_INSTANCE_SHIFT)

/* the GuC arrays don't include OTHER_CLASS */
static u8 engine_class_guc_class_map[] = {
	[RENDER_CLASS]            = GUC_RENDER_CLASS,
	[COPY_ENGINE_CLASS]       = GUC_BLITTER_CLASS,
	[VIDEO_DECODE_CLASS]      = GUC_VIDEO_CLASS,
	[VIDEO_ENHANCEMENT_CLASS] = GUC_VIDEOENHANCE_CLASS,
	[COMPUTE_CLASS]           = GUC_COMPUTE_CLASS,
};

static u8 guc_class_engine_class_map[] = {
	[GUC_RENDER_CLASS]       = RENDER_CLASS,
	[GUC_BLITTER_CLASS]      = COPY_ENGINE_CLASS,
	[GUC_VIDEO_CLASS]        = VIDEO_DECODE_CLASS,
	[GUC_VIDEOENHANCE_CLASS] = VIDEO_ENHANCEMENT_CLASS,
	[GUC_COMPUTE_CLASS]      = COMPUTE_CLASS,
};
#define SLPC_EVENT(id, c) (\
FIELD_PREP(HOST2GUC_PC_SLPC_REQUEST_MSG_1_EVENT_ID, id) | \
FIELD_PREP(HOST2GUC_PC_SLPC_REQUEST_MSG_1_EVENT_ARGC, c) \
)

static inline u8 engine_class_to_guc_class(u8 class)
{
	BUILD_BUG_ON(ARRAY_SIZE(engine_class_guc_class_map) != MAX_ENGINE_CLASS + 1);
	GEM_BUG_ON(class > MAX_ENGINE_CLASS || class == OTHER_CLASS);

	return engine_class_guc_class_map[class];
}

static inline u8 guc_class_to_engine_class(u8 guc_class)
{
	BUILD_BUG_ON(ARRAY_SIZE(guc_class_engine_class_map) != GUC_LAST_ENGINE_CLASS + 1);
	GEM_BUG_ON(guc_class > GUC_LAST_ENGINE_CLASS);

	return guc_class_engine_class_map[guc_class];
}

/* Work item for submitting workloads into work queue of GuC. */
struct guc_wq_item {
	u32 header;
	u32 context_desc;
	u32 submit_element_info;
	u32 fence_id;
} __packed;

struct guc_sched_wq_desc {
	u32 head;
	u32 tail;
	u32 error_offset;
	u32 wq_status;
	u32 reserved[28];
} __packed;

/* Helper for context registration H2G */
struct guc_ctxt_registration_info {
	u32 flags;
	u32 context_idx;
	u32 engine_class;
	u32 engine_submit_mask;
	u32 wq_desc_lo;
	u32 wq_desc_hi;
	u32 wq_base_lo;
	u32 wq_base_hi;
	u32 wq_size;
	u32 hwlrca_lo;
	u32 hwlrca_hi;
};
#define CONTEXT_REGISTRATION_FLAG_KMD	BIT(0)

/* 32-bit KLV structure as used by policy updates and others */
struct guc_klv_generic_dw_t {
	u32 kl;
	u32 value;
} __packed;

/* Format of the UPDATE_CONTEXT_POLICIES H2G data packet */
struct guc_update_context_policy_header {
	u32 action;
	u32 ctx_id;
} __packed;

struct guc_update_context_policy {
	struct guc_update_context_policy_header header;
	struct guc_klv_generic_dw_t klv[GUC_CONTEXT_POLICIES_KLV_NUM_IDS];
} __packed;

#define GUC_POWER_UNSPECIFIED	0
#define GUC_POWER_D0		1
#define GUC_POWER_D1		2
#define GUC_POWER_D2		3
#define GUC_POWER_D3		4

/* Scheduling policy settings */

#define GLOBAL_POLICY_MAX_NUM_WI 15

/* Don't reset an engine upon preemption failure */
#define GLOBAL_POLICY_DISABLE_ENGINE_RESET				BIT(0)

#define GLOBAL_POLICY_DEFAULT_DPC_PROMOTE_TIME_US 500000

struct guc_policies {
	u32 submission_queue_depth[GUC_MAX_ENGINE_CLASSES];
	/* In micro seconds. How much time to allow before DPC processing is
	 * called back via interrupt (to prevent DPC queue drain starving).
	 * Typically 1000s of micro seconds (example only, not granularity). */
	u32 dpc_promote_time;

	/* Must be set to take these new values. */
	u32 is_valid;

	/* Max number of WIs to process per call. A large value may keep CS
	 * idle. */
	u32 max_num_work_items;

	u32 global_flags;
	u32 reserved[4];
} __packed;

/* GuC MMIO reg state struct */
struct guc_mmio_reg {
	u32 offset;
	u32 value;
	u32 flags;
	u32 mask;
#define GUC_REGSET_MASKED		BIT(0)
#define GUC_REGSET_MASKED_WITH_VALUE	BIT(2)
#define GUC_REGSET_RESTORE_ONLY		BIT(3)
} __packed;

/* GuC register sets */
struct guc_mmio_reg_set {
	u32 address;
	u16 count;
	u16 reserved;
} __packed;

/* HW info */
struct guc_gt_system_info {
	u8 mapping_table[GUC_MAX_ENGINE_CLASSES][GUC_MAX_INSTANCES_PER_CLASS];
	u32 engine_enabled_masks[GUC_MAX_ENGINE_CLASSES];
	u32 generic_gt_sysinfo[GUC_GENERIC_GT_SYSINFO_MAX];
} __packed;

enum {
	GUC_CAPTURE_LIST_INDEX_PF = 0,
	GUC_CAPTURE_LIST_INDEX_VF = 1,
	GUC_CAPTURE_LIST_INDEX_MAX = 2,
};

/* GuC Additional Data Struct */
struct guc_ads {
	struct guc_mmio_reg_set reg_state_list[GUC_MAX_ENGINE_CLASSES][GUC_MAX_INSTANCES_PER_CLASS];
	u32 reserved0;
	u32 scheduler_policies;
	u32 gt_system_info;
	u32 reserved1;
	u32 control_data;
	u32 golden_context_lrca[GUC_MAX_ENGINE_CLASSES];
	u32 eng_state_size[GUC_MAX_ENGINE_CLASSES];
	u32 private_data;
	u32 reserved2;
	u32 capture_instance[GUC_CAPTURE_LIST_INDEX_MAX][GUC_MAX_ENGINE_CLASSES];
	u32 capture_class[GUC_CAPTURE_LIST_INDEX_MAX][GUC_MAX_ENGINE_CLASSES];
	u32 capture_global[GUC_CAPTURE_LIST_INDEX_MAX];
	u32 reserved[14];
} __packed;

/* Engine usage stats */
struct guc_engine_usage_record {
	u32 current_context_index;
	u32 last_switch_in_stamp;
	u32 reserved0;
	u32 total_runtime;
	u32 reserved1[4];
} __packed;

struct guc_engine_usage {
	struct guc_engine_usage_record engines[GUC_MAX_ENGINE_CLASSES][GUC_MAX_INSTANCES_PER_CLASS];
} __packed;

/* GuC logging structures */

enum guc_log_buffer_type {
	GUC_DEBUG_LOG_BUFFER,
	GUC_CRASH_DUMP_LOG_BUFFER,
	GUC_CAPTURE_LOG_BUFFER,
	GUC_MAX_LOG_BUFFER
};

/**
 * struct guc_log_buffer_state - GuC log buffer state
 *
 * Below state structure is used for coordination of retrieval of GuC firmware
 * logs. Separate state is maintained for each log buffer type.
 * read_ptr points to the location where i915 read last in log buffer and
 * is read only for GuC firmware. write_ptr is incremented by GuC with number
 * of bytes written for each log entry and is read only for i915.
 * When any type of log buffer becomes half full, GuC sends a flush interrupt.
 * GuC firmware expects that while it is writing to 2nd half of the buffer,
 * first half would get consumed by Host and then get a flush completed
 * acknowledgment from Host, so that it does not end up doing any overwrite
 * causing loss of logs. So when buffer gets half filled & i915 has requested
 * for interrupt, GuC will set flush_to_file field, set the sampled_write_ptr
 * to the value of write_ptr and raise the interrupt.
 * On receiving the interrupt i915 should read the buffer, clear flush_to_file
 * field and also update read_ptr with the value of sample_write_ptr, before
 * sending an acknowledgment to GuC. marker & version fields are for internal
 * usage of GuC and opaque to i915. buffer_full_cnt field is incremented every
 * time GuC detects the log buffer overflow.
 */
struct guc_log_buffer_state {
	u32 marker[2];
	u32 read_ptr;
	u32 write_ptr;
	u32 size;
	u32 sampled_write_ptr;
	u32 wrap_offset;
	union {
		struct {
			u32 flush_to_file:1;
			u32 buffer_full_cnt:4;
			u32 reserved:27;
		};
		u32 flags;
	};
	u32 version;
} __packed;

struct guc_ctx_report {
	u32 report_return_status;
	u32 reserved1[64];
	u32 affected_count;
	u32 reserved2[2];
} __packed;

/* GuC Shared Context Data Struct */
struct guc_shared_ctx_data {
	u32 addr_of_last_preempted_data_low;
	u32 addr_of_last_preempted_data_high;
	u32 addr_of_last_preempted_data_high_tmp;
	u32 padding;
	u32 is_mapped_to_proxy;
	u32 proxy_ctx_id;
	u32 engine_reset_ctx_id;
	u32 media_reset_count;
	u32 reserved1[8];
	u32 uk_last_ctx_switch_reason;
	u32 was_reset;
	u32 lrca_gpu_addr;
	u64 execlist_ctx;
	u32 reserved2[66];
	struct guc_ctx_report preempt_ctx_report[GUC_MAX_ENGINES_NUM];
} __packed;

/* This action will be programmed in C1BC - SOFT_SCRATCH_15_REG */
enum intel_guc_recv_message {
	INTEL_GUC_RECV_MSG_CRASH_DUMP_POSTED = BIT(1),
	INTEL_GUC_RECV_MSG_EXCEPTION = BIT(30),
};

#define INTEL_GUC_SUPPORTS_TLB_INVALIDATION(guc) \
	((intel_guc_ct_enabled(&(guc)->ct)) && \
	 (intel_guc_submission_is_used(guc)) && \
	 (GRAPHICS_VER(guc_to_gt((guc))->i915) >= 12))
#define INTEL_GUC_SUPPORTS_TLB_INVALIDATION_ENGINE(guc) \
	INTEL_GUC_SUPPORTS_TLB_INVALIDATION(guc)
#define INTEL_GUC_SUPPORTS_TLB_INVALIDATION_FULL(guc) \
	(INTEL_GUC_SUPPORTS_TLB_INVALIDATION(guc) && \
	HAS_SELECTIVE_TLB_INVALIDATION(guc_to_gt(guc)->i915))
#define INTEL_GUC_SUPPORTS_TLB_INVALIDATION_SELECTIVE(guc) \
	(INTEL_GUC_SUPPORTS_TLB_INVALIDATION(guc) && \
	HAS_SELECTIVE_TLB_INVALIDATION(guc_to_gt(guc)->i915))

#endif
