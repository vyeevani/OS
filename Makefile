GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386
objects = loader.o kernel.o

%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

reality_kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: reality_kernel.bin
	sudo cp $< /boot/reality_kernel.bin

reality.iso: reality_kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Reality OS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/reality_kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso

clean: reality.iso
	rm -rf iso
	rm *.o
	rm *.bin
	rm *.iso
