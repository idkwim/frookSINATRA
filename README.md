<pre>
<b>FrookSINATRA</b>
is a kernel driver and a Virtualbox(Hypervisor) patch to make possible hook of the LSTAR, even with patchguard (Windows 8.1 on July 2014) activated.


<b>DRIVER:</b>
On Load, the driver:
	1. save the old lSTAR,
	2. ask the hypervisor (MSR Knoocking) to save the old LSTAR too,
	3. write the LSTAR to is hook,
	4. ... Working here

On unload, the driver:
	6. restore the old LSTAR
	5. ask the hypervisor to forget the LSTAR
	
	
<b>HYPERVISOR:</b>
	I changed the virtualbox (Dirty patch again) VT-X hypervisor HMVMXR0.cpp, to intercept read and write of MSR.
	When the kernel (Linux or Windows) write a magic value on a magic MSR, the LSTAR is stored.
	When Patchguard ask the register asm("rdmsr 0xC000005"), the hypervisor intercept the read, and give the value stored, even if it was hooked by a driver :p
	
	This is working because when a sysenter/syscall is done, the LSTAR MSR isn't read via rdmsr, and hypervisor isn't call. So the given LSTAR value is the hook (if hooked).
	

<b>TODO:</b>
	Make a version where all the thing is done in hypervisor, write the hook EIP in a magic MSR...
	Make it working with AMD, 2	lines to change,
	Could be integrated in (this nice tool) https://github.com/zer0mem/MiniHyperVisorProject, to make it working on a live Windows (pluePill+Intercept R/W MSR+frookSINATRA = Rootkit ;p)
</pre>
