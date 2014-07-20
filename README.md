FrookSINATRA is a kernel driver and a Virtualbox(Hypervisor) patch to make possible hook of the LSTAR, even with patchguard (Windows 8.1 on July 2014) activated.


Driver:
On Load, the driver:<br/>
	1. save the old lSTAR,<br/>
	2. ask the hypervisor (MSR Knoocking) to save the old LSTAR too,<br/>
	3. write the LSTAR to is hook,<br/>
	4. ... Working here<br/>

On unload, the driver:<br/>
	6. restore the old LSTAR<br/>
	5. ask the hypervisor to forget the LSTAR<br/>
	
	
Hypervisor:<br/>
	I changed the virtualbox (Dirty patch again) VT-X hypervisor HMVMXR0.cpp, to intercept read and write of MSR.<br/>
	When the kernel (Linux or Windows) write a magic value on a magic MSR, the LSTAR is stored.<br/>
	When Patchguard ask the register asm("rdmsr 0xC000005"), the hypervisor intercept the read, and give the value stored, even if it was hooked by a driver :p<br/>
	
	This is working because when a sysenter/syscall is done, the msr isn't read, and hypervisor isn't call, so the MSR setted as hook EIP and not detected<br/>
	

TODO:<br/>
	Do a version where all the thing is done in hypervisor, write the hook EIP in a magic MSR...<br/>
	Make it working with AMD, 2	lines to change,<br/>
	Could be integrated in https://github.com/zer0mem/MiniHyperVisorProject, to make it working on a live Windows (pluePill+Intercept R/W MSR+frookSINATRA = Rootkit ;p)<br/>
