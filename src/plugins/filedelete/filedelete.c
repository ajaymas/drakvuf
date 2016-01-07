/*********************IMPORTANT DRAKVUF LICENSE TERMS***********************
 *                                                                         *
 * DRAKVUF Dynamic Malware Analysis System (C) 2014-2015 Tamas K Lengyel.  *
 * Tamas K Lengyel is hereinafter referred to as the author.               *
 * This program is free software; you may redistribute and/or modify it    *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; Version 2 ("GPL"), BUT ONLY WITH ALL OF THE   *
 * CLARIFICATIONS AND EXCEPTIONS DESCRIBED HEREIN.  This guarantees your   *
 * right to use, modify, and redistribute this software under certain      *
 * conditions.  If you wish to embed DRAKVUF technology into proprietary   *
 * software, alternative licenses can be aquired from the author.          *
 *                                                                         *
 * Note that the GPL places important restrictions on "derivative works",  *
 * yet it does not provide a detailed definition of that term.  To avoid   *
 * misunderstandings, we interpret that term as broadly as copyright law   *
 * allows.  For example, we consider an application to constitute a        *
 * derivative work for the purpose of this license if it does any of the   *
 * following with any software or content covered by this license          *
 * ("Covered Software"):                                                   *
 *                                                                         *
 * o Integrates source code from Covered Software.                         *
 *                                                                         *
 * o Reads or includes copyrighted data files.                             *
 *                                                                         *
 * o Is designed specifically to execute Covered Software and parse the    *
 * results (as opposed to typical shell or execution-menu apps, which will *
 * execute anything you tell them to).                                     *
 *                                                                         *
 * o Includes Covered Software in a proprietary executable installer.  The *
 * installers produced by InstallShield are an example of this.  Including *
 * DRAKVUF with other software in compressed or archival form does not     *
 * trigger this provision, provided appropriate open source decompression  *
 * or de-archiving software is widely available for no charge.  For the    *
 * purposes of this license, an installer is considered to include Covered *
 * Software even if it actually retrieves a copy of Covered Software from  *
 * another source during runtime (such as by downloading it from the       *
 * Internet).                                                              *
 *                                                                         *
 * o Links (statically or dynamically) to a library which does any of the  *
 * above.                                                                  *
 *                                                                         *
 * o Executes a helper program, module, or script to do any of the above.  *
 *                                                                         *
 * This list is not exclusive, but is meant to clarify our interpretation  *
 * of derived works with some common examples.  Other people may interpret *
 * the plain GPL differently, so we consider this a special exception to   *
 * the GPL that we apply to Covered Software.  Works which meet any of     *
 * these conditions must conform to all of the terms of this license,      *
 * particularly including the GPL Section 3 requirements of providing      *
 * source code and allowing free redistribution of the work as a whole.    *
 *                                                                         *
 * Any redistribution of Covered Software, including any derived works,    *
 * must obey and carry forward all of the terms of this license, including *
 * obeying all GPL rules and restrictions.  For example, source code of    *
 * the whole work must be provided and free redistribution must be         *
 * allowed.  All GPL references to "this License", are to be treated as    *
 * including the terms and conditions of this license text as well.        *
 *                                                                         *
 * Because this license imposes special exceptions to the GPL, Covered     *
 * Work may not be combined (even as part of a larger work) with plain GPL *
 * software.  The terms, conditions, and exceptions of this license must   *
 * be included as well.  This license is incompatible with some other open *
 * source licenses as well.  In some cases we can relicense portions of    *
 * DRAKVUF or grant special permissions to use it in other open source     *
 * software.  Please contact tamas.k.lengyel@gmail.com with any such       *
 * requests.  Similarly, we don't incorporate incompatible open source     *
 * software into Covered Software without special permission from the      *
 * copyright holders.                                                      *
 *                                                                         *
 * If you have any questions about the licensing restrictions on using     *
 * DRAKVUF in other works, are happy to help.  As mentioned above,         *
 * alternative license can be requested from the author to integrate       *
 * DRAKVUF into proprietary applications and appliances.  Please email     *
 * tamas.k.lengyel@gmail.com for further information.                      *
 *                                                                         *
 * If you have received a written license agreement or contract for        *
 * Covered Software stating terms other than these, you may choose to use  *
 * and redistribute Covered Software under those terms instead of these.   *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes.          *
 *                                                                         *
 * Source code also allows you to port DRAKVUF to new platforms, fix bugs, *
 * and add new features.  You are highly encouraged to submit your changes *
 * on https://github.com/tklengyel/drakvuf, or by other methods.           *
 * By sending these changes, it is understood (unless you specify          *
 * otherwise) that you are offering unlimited, non-exclusive right to      *
 * reuse, modify, and relicense the code.  DRAKVUF will always be          *
 * available Open Source, but this is important because the inability to   *
 * relicense code has caused devastating problems for other Free Software  *
 * projects (such as KDE and NASM).                                        *
 * To specify special license conditions of your contributions, just say   *
 * so when you send them.                                                  *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the DRAKVUF   *
 * license file for more details (it's in a COPYING file included with     *
 * DRAKVUF, and also available from                                        *
 * https://github.com/tklengyel/drakvuf/COPYING)                           *
 *                                                                         *
 ***************************************************************************/

#include <glib.h>
#include <config.h>
#include "../plugins.h"

#define FILE_DISPOSITION_INFORMATION 13

static const char *dump_folder;
static output_format_t format;
static event_response_t cb(drakvuf_t drakvuf, drakvuf_trap_info_t *info);
static page_mode_t pm;
static uint32_t domid;

enum offset {
    FILE_OBJECT_FILENAME,
    HANDLE_TABLE_HANDLECOUNT,
    KPCR_PRCB,
    KPRCB_CURRENTTHREAD,
    KTHREAD_PROCESS,
    OBJECT_HEADER_TYPEINDEX,
    OBJECT_HEADER_BODY,
    UNICODE_STRING_LENGTH,
    UNICODE_STRING_BUFFER,
    __OFFSET_MAX
};

static const char *offset_names[__OFFSET_MAX][2] = {
    [FILE_OBJECT_FILENAME] = {"_FILE_OBJECT", "FileName"},
    [HANDLE_TABLE_HANDLECOUNT] = {"_HANDLE_TABLE", "HandleCount" },
    [KPRCB_CURRENTTHREAD] = { "_KPRCB", "CurrentThread" },
    [KTHREAD_PROCESS] = {"_KTHREAD", "Process" },
    [OBJECT_HEADER_TYPEINDEX] = { "_OBJECT_HEADER", "TypeIndex" },
    [OBJECT_HEADER_BODY] = { "_OBJECT_HEADER", "Body" },
    [UNICODE_STRING_LENGTH] = {"_UNICODE_STRING", "Length" },
    [UNICODE_STRING_BUFFER] = {"_UNICODE_STRING", "Buffer" },
};

#define WIN7_TYPEINDEX_LAST 44
#define VOL_DUMPFILES "%s %s -l vmi://domid/%u --profile=%s -Q 0x%lx -D %s -n dumpfiles 2>&1"
#define PROFILE32 "Win7SP1x86"
#define PROFILE64 "Win7SP1x64"

static size_t offsets[__OFFSET_MAX];

static drakvuf_trap_t traps[4] = {
    [0 ... 3] = { .lookup_type = LOOKUP_PID,
                  .u.pid = 4,
                  .addr_type = ADDR_RVA,
                  .type = BREAKPOINT,
                  .module = "ntoskrnl.exe" }
};

void volatility_extract_file(drakvuf_t drakvuf, addr_t file_object) {

    const char* profile = NULL;
    if (pm == VMI_PM_IA32E)
        profile = PROFILE64;
    else
        profile = PROFILE32;

    char *command = g_malloc0(
            snprintf(NULL, 0, VOL_DUMPFILES, PYTHON, VOLATILITY, domid,
                     profile, file_object, dump_folder
                    )+ 1);
    sprintf(command, VOL_DUMPFILES, PYTHON, VOLATILITY, domid, profile,
            file_object, dump_folder);

    g_spawn_command_line_sync(command, NULL, NULL, NULL, NULL);
    free(command);
}

/*
 * The approach where the system process list es enumerated looking for
 * the matching cr3 value in each _EPROCESS struct is not going to work
 * if a DKOM attack unhooks the _EPROCESS struct.
 *
 * We can access the _EPROCESS structure by reading the FS_BASE register on x86
 * or the GS_BASE register on x64, which contains the _KPCR.
 *
 * FS/GS -> _KPCR._KPRCB.CurrentThread -> _ETHREAD._KTHREAD.Process = _EPROCESS
 *
 * Also see: http://www.csee.umbc.edu/~stephens/SECURITY/491M/HiddenProcesses.ppt
 */
static void grab_file_by_handle(drakvuf_t drakvuf, uint32_t vcpu_id, vmi_instance_t vmi,
                                page_mode_t pm,
                                drakvuf_trap_info_t *info, addr_t handle)
{
    uint8_t type_index = 0;
    addr_t thread, process;
    /*
     * fs_base/gs_base in the info->regs structure are not actually filled in
     * by Xen for vm_events, so we need to manually ask for these each time
     */
    reg_t fsgs;
    if(pm == VMI_PM_IA32E)  {
        vmi_get_vcpureg(vmi, &fsgs, GS_BASE, vcpu_id);
    } else {
        vmi_get_vcpureg(vmi, &fsgs, FS_BASE, vcpu_id);
    }

    vmi_read_addr_va(vmi, fsgs + offsets[KPCR_PRCB] + offsets[KPRCB_CURRENTTHREAD],
                     0, &thread);
    vmi_read_addr_va(vmi, thread + offsets[KTHREAD_PROCESS],
                     0, &process);

    // TODO: verify that the dtb in the _EPROCESS is the same as the cr3?

    addr_t obj = drakvuf_get_obj_by_handle(drakvuf, process, handle);

    if (!obj)
        return;

    access_context_t ctx = {
        .translate_mechanism = VMI_TM_PROCESS_DTB,
        .addr = obj + offsets[OBJECT_HEADER_TYPEINDEX],
        .dtb = info->regs->cr3
    };

    if (VMI_FAILURE == vmi_read_8(vmi, &ctx, &type_index))
        return;

    if (type_index >= WIN7_TYPEINDEX_LAST || type_index != 28)
        return;

    addr_t file = obj + offsets[OBJECT_HEADER_BODY];
    addr_t file_pa = vmi_pagetable_lookup(vmi, info->regs->cr3, file);
    addr_t filename = file + offsets[FILE_OBJECT_FILENAME];

    uint16_t length = 0;
    addr_t buffer = 0;

    ctx.addr = filename + offsets[UNICODE_STRING_BUFFER];
    vmi_read_addr(vmi, &ctx, &buffer);

    ctx.addr = filename + offsets[UNICODE_STRING_LENGTH];
    vmi_read_16(vmi, &ctx, &length);

    if (length && buffer) {

        unicode_string_t str = { .contents = NULL };
        str.length = length;
        str.encoding = "UTF-16";
        str.contents = malloc(length);

        ctx.addr = buffer;
        vmi_read(vmi, &ctx, str.contents, length);

        unicode_string_t str2 = { .contents = NULL };
        status_t rc = vmi_convert_str_encoding(&str, &str2, "UTF-8");
        if (rc == VMI_SUCCESS) {
            //printf("\tExtracting file: %s\n", str2.contents);

            volatility_extract_file(drakvuf, file_pa);

            free(str2.contents);
        }

        free(str.contents);
    }
}

static event_response_t setinformation(drakvuf_t drakvuf, drakvuf_trap_info_t *info) {

    vmi_instance_t vmi = drakvuf_lock_and_get_vmi(drakvuf);

    access_context_t ctx = {
        .translate_mechanism = VMI_TM_PROCESS_DTB,
        .dtb = info->regs->cr3
    };

    uint32_t fileinfoclass = 0;
    reg_t handle = 0, fileinfo = 0, length = 0;

    if (pm == VMI_PM_IA32E) {
        handle = info->regs->rcx;
        fileinfo = info->regs->r8;
        length = info->regs->r9;

        ctx.addr = info->regs->rsp + 5 * sizeof(addr_t); // addr of fileinfoclass
        vmi_read_32(vmi, &ctx, &fileinfoclass);
    } else {
        ctx.addr = info->regs->rsp + sizeof(uint32_t);
        vmi_read_32(vmi, &ctx, (uint32_t*) &handle);
        ctx.addr += 2 * sizeof(uint32_t);
        vmi_read_32(vmi, &ctx, (uint32_t*) &fileinfo);
        ctx.addr += sizeof(uint32_t);
        vmi_read_32(vmi, &ctx, (uint32_t*) &length);
        ctx.addr += sizeof(uint32_t);
        vmi_read_32(vmi, &ctx, &fileinfoclass);
    }

    if (fileinfoclass == FILE_DISPOSITION_INFORMATION && length == 1) {
        uint8_t del = 0;
        ctx.addr = fileinfo;
        vmi_read_8(vmi, &ctx, &del);
        if (del) {
            //printf("DELETE FILE _FILE_OBJECT Handle: 0x%lx.\n", handle);
            grab_file_by_handle(drakvuf, info->vcpu, vmi, pm, info, handle);
        }
    }

    drakvuf_release_vmi(drakvuf);
    return 0;
}

int plugin_filedelete_init(drakvuf_t drakvuf,
                           const struct filedelete_config *c)
{
    vmi_instance_t vmi = drakvuf_lock_and_get_vmi(drakvuf);
    pm = vmi_get_page_mode(vmi);
    domid = vmi_get_vmid(vmi);
    drakvuf_release_vmi(drakvuf);

    dump_folder = c->dump_folder ? c->dump_folder : "/tmp";
    format = drakvuf_get_output_format(drakvuf);

    traps[0].u2.rva = drakvuf_get_function_rva(c->rekall_profile, "NtSetInformationFile");
    traps[0].name = "NtSetInformationFile";
    traps[0].cb = setinformation;
    traps[1].u2.rva = drakvuf_get_function_rva(c->rekall_profile, "ZwSetInformationFile");
    traps[1].name = "ZwSetInformationFile";
    traps[1].cb = setinformation;
    /*traps[2].u2.rva = drakvuf_get_function_rva(c->rekall_profile, "NtDeleteFile");
    traps[2].name = "NtDeleteFile";
    traps[3].u2.rva = drakvuf_get_function_rva(c->rekall_profile, "ZwDeleteFile");
    traps[3].name = "ZwDeleteFile";*/

    int i;
    for(i=0;i<__OFFSET_MAX;i++) {
        if(i==KPCR_PRCB) {
            if(pm == VMI_PM_IA32E)
                windows_system_map_lookup(
                    c->rekall_profile, "_KPCR", "Prcb", &offsets[KPCR_PRCB], NULL);
            else
                windows_system_map_lookup(
                    c->rekall_profile, "_KPCR", "PrcbData", &offsets[KPCR_PRCB], NULL);
        } else {
            windows_system_map_lookup(
                c->rekall_profile, offset_names[i][0], offset_names[i][1], &offsets[i], NULL);
        }
    }

    return 1;
}

int plugin_filedelete_start(drakvuf_t drakvuf) {
    drakvuf_add_trap(drakvuf, &traps[0]);
    //drakvuf_add_trap(drakvuf, &traps[1]);
    //drakvuf_add_trap(drakvuf, &traps[2]);
    //drakvuf_add_trap(drakvuf, &traps[3]);
    return 1;
}

int plugin_filedelete_close(drakvuf_t drakvuf) {
    return 1;
}