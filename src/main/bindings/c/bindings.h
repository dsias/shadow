/*
 * The Shadow Simulator
 * See LICENSE for licensing information
 */
// clang-format off


#ifndef main_bindings_h
#define main_bindings_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "main/bindings/c/bindings-opaque.h"
#include "main/host/descriptor/descriptor_types.h"
#include "main/host/status_listener.h"
#include "main/host/syscall_handler.h"
#include "main/host/syscall_types.h"
#include "main/host/thread.h"

void rust_logging_init(void);

// The new compat descriptor takes ownership of the reference to the legacy descriptor and
// does not increment its ref count, but will decrement the ref count when this compat
// descriptor is freed/dropped.
CompatDescriptor *compatdescriptor_fromLegacy(LegacyDescriptor *legacy_descriptor);

// If the compat descriptor is a legacy descriptor, returns a pointer to the legacy
// descriptor object. Otherwise returns NULL. The legacy descriptor's ref count is not
// modified, so the pointer must not outlive the lifetime of the compat descriptor.
LegacyDescriptor *compatdescriptor_asLegacy(const CompatDescriptor *descriptor);

// When the compat descriptor is freed/dropped, it will decrement the legacy descriptor's
// ref count.
void compatdescriptor_free(CompatDescriptor *descriptor);

// This is a no-op for non-legacy descriptors.
void compatdescriptor_setHandle(CompatDescriptor *descriptor, int handle);

// If the compat descriptor is a new descriptor, returns a pointer to the reference-counted
// posix file object. Otherwise returns NULL. The posix file object's ref count is not
// modified, so the pointer must not outlive the lifetime of the compat descriptor.
const PosixFileArc *compatdescriptor_borrowPosixFile(CompatDescriptor *descriptor);

// If the compat descriptor is a new descriptor, returns a pointer to the reference-counted
// posix file object. Otherwise returns NULL. The posix file object's ref count is
// incremented, so the pointer must always later be passed to `posixfile_drop()`, otherwise
// the memory will leak.
const PosixFileArc *compatdescriptor_newRefPosixFile(CompatDescriptor *descriptor);

// Decrement the ref count of the posix file object. The pointer must not be used after
// calling this function.
void posixfile_drop(const PosixFileArc *file);

// Get the status of the posix file object.
Status posixfile_getStatus(const PosixFileArc *file);

// Add a status listener to the posix file object. This will increment the status
// listener's ref count, and will decrement the ref count when this status listener is
// removed or when the posix file is freed/dropped.
void posixfile_addListener(const PosixFileArc *file, StatusListener *listener);

// Remove a listener from the posix file object.
void posixfile_removeListener(const PosixFileArc *file, StatusListener *listener);

// # Safety
// * `thread` must point to a valid object.
MemoryManager *memorymanager_new(Thread *thread);

// # Safety
// * `mm` must point to a valid object.
void memorymanager_free(MemoryManager *mm);

// Get a readable pointer to the plugin's memory via mapping, or via the thread APIs.
// # Safety
// * `mm` and `thread` must point to valid objects.
const void *memorymanager_getReadablePtr(MemoryManager *memory_manager,
                                         Thread *thread,
                                         PluginPtr plugin_src,
                                         uintptr_t n);

// Get a writeable pointer to the plugin's memory via mapping, or via the thread APIs.
// # Safety
// * `mm` and `thread` must point to valid objects.
void *memorymanager_getWriteablePtr(MemoryManager *memory_manager,
                                    Thread *thread,
                                    PluginPtr plugin_src,
                                    uintptr_t n);

// Get a mutable pointer to the plugin's memory via mapping, or via the thread APIs.
// # Safety
// * `mm` and `thread` must point to valid objects.
void *memorymanager_getMutablePtr(MemoryManager *memory_manager,
                                  Thread *thread,
                                  PluginPtr plugin_src,
                                  uintptr_t n);

// Fully handles the `brk` syscall, keeping the "heap" mapped in our shared mem file.
SysCallReg memorymanager_handleBrk(MemoryManager *memory_manager,
                                   Thread *thread,
                                   PluginPtr plugin_src);

// Fully handles the `mmap` syscall
SysCallReg memorymanager_handleMmap(MemoryManager *memory_manager,
                                    Thread *thread,
                                    PluginPtr addr,
                                    uintptr_t len,
                                    int32_t prot,
                                    int32_t flags,
                                    int32_t fd,
                                    int64_t offset);

// Fully handles the `munmap` syscall
SysCallReg memorymanager_handleMunmap(MemoryManager *memory_manager,
                                      Thread *thread,
                                      PluginPtr addr,
                                      uintptr_t len);

SysCallReg memorymanager_handleMremap(MemoryManager *memory_manager,
                                      Thread *thread,
                                      PluginPtr old_addr,
                                      uintptr_t old_size,
                                      uintptr_t new_size,
                                      int32_t flags,
                                      PluginPtr new_addr);

SysCallReg memorymanager_handleMprotect(MemoryManager *memory_manager,
                                        Thread *thread,
                                        PluginPtr addr,
                                        uintptr_t size,
                                        int32_t prot);

SysCallReturn rustsyscallhandler_close(SysCallHandler *sys, const SysCallArgs *args);

SysCallReturn rustsyscallhandler_dup(SysCallHandler *sys, const SysCallArgs *args);

SysCallReturn rustsyscallhandler_read(SysCallHandler *sys, const SysCallArgs *args);

SysCallReturn rustsyscallhandler_write(SysCallHandler *sys, const SysCallArgs *args);

SysCallReturn rustsyscallhandler_pipe(SysCallHandler *sys, const SysCallArgs *args);

SysCallReturn rustsyscallhandler_pipe2(SysCallHandler *sys, const SysCallArgs *args);

ByteQueue *bytequeue_new(size_t chunk_size);

void bytequeue_free(ByteQueue *bq_ptr);

size_t bytequeue_len(ByteQueue *bq);

void bytequeue_push(ByteQueue *bq, const unsigned char *src, size_t len);

size_t bytequeue_pop(ByteQueue *bq, unsigned char *dst, size_t len);

#endif /* main_bindings_h */
