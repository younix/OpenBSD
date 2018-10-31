/* Public domain. */
#ifndef I915_TRACE_H
#define I915_TRACE_H

#include "intel_drv.h"

#define trace_i915_context_create(a)
#define trace_i915_context_free(a)
#define trace_i915_flip_complete(a, b)
#define trace_i915_flip_request(a, b)
#define trace_i915_gem_evict(a, b, c, d)
#define trace_i915_gem_evict_vm(a)
#define trace_i915_gem_object_change_domain(a, b, c)
#define trace_i915_gem_object_clflush(a)
#define trace_i915_gem_object_create(a)
#define trace_i915_gem_object_destroy(a)
#define trace_i915_gem_object_fault(a, b, c, d)
#define trace_i915_gem_object_pread(a, b, c)
#define trace_i915_gem_object_pwrite(a, b, c)
#define trace_i915_gem_request_add(a)
#define trace_i915_gem_request_notify(a)
#define trace_i915_gem_request_retire(a)
#define trace_i915_gem_request_wait_begin(a)
#define trace_i915_gem_request_wait_end(a)
#define trace_i915_gem_ring_dispatch(a, b)
#define trace_i915_gem_ring_flush(a, b, c)
#define trace_i915_gem_ring_flush(a, b, c)
#define trace_i915_gem_ring_sync_to(a, b, c)
#define trace_i915_page_directory_entry_alloc(a, b, c, d)
#define trace_i915_page_directory_pointer_entry_alloc(a, b, c, d)
#define trace_i915_page_table_entry_alloc(a, b, c, d)
#define trace_i915_page_table_entry_map(a, b, c, d, e, f)
#define trace_i915_pipe_update_end(a, b, c)
#define trace_i915_pipe_update_start(a)
#define trace_i915_pipe_update_vblank_evaded(a)
#define trace_i915_ppgtt_create(a)
#define trace_i915_ppgtt_release(a)
#define trace_i915_reg_rw(a, b, c, d, e)
#define trace_i915_va_alloc(a, b, c, d)
#define trace_i915_vma_bind(a, b)
#define trace_i915_vma_unbind(a)
#define trace_intel_gpu_freq_change(a)
#define trace_switch_mm(a, b)

#endif
