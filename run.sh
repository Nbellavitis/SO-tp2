#!/bin/bash


#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk

#qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int -soundhw pcspk

#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev sdl,id=audio0 -machine pcspk-audiodev=audio0

#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0

#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev $audio,id=audio0 -machine pcspk-audiodev=audio0

#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev $audio,id=audio0 -soundhw pcspk

#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=audio0 -soundhw pcspk

qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0