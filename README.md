<pre>
<b>FrookSINATRA</b>
is a kernel driver and a Virtualbox(Hypervisor) patch to make possible hook of the LSTAR, even with patchguard (Windows 8.1 on July 2014) activated.


<b>DRIVER:</b>
On Load, the driver:
	1. save the original lSTAR,
	2. ask the hypervisor (MSR Knocking) to save the original LSTAR too,
	3. write the LSTAR to with the address of the hook function,
	4. ... Working here ...

On unload, the driver:
	6. restore the old LSTAR
	5. ask the hypervisor to forget the old LSTAR
	
	
<b>HYPERVISOR:</b>
	I changed the virtualbox (Dirty patch again) VT-X hypervisor HMVMXR0.cpp, to intercept read and write of MSR.
	When the kernel (Linux or Windows) write a magic value on a magic MSR, the LSTAR is stored.
	When Patchguard ask the register asm("rdmsr 0xC000005") http://pastebin.com/mGbFHkk5, the hypervisor intercept the read, and give the original LSTAR value (legit one), even if it was hooked by the driver !
	
	This is working because when a sysenter/syscall is made, the LSTAR MSR isn't read via rdmsr instruction, but read by the CPU itself, and hypervisor isn't call. So the instruction flow is redirected by the true value of the LSTAR, the hook function if hooked.
	
<b>NOTE:</b>
	The driver can be loaded with dsefix (http://www.kernelmode.info/forum/viewtopic.php?f=11&t=3322)
	The driver can work on Windows 7 x64 without the hypervisor

<b>TODO:</b>
	* Make a version where all the thing is done in hypervisor, write the hook EIP in a magic MSR...
	* Make it working with AMD, 2 lines to change,
	* Could be integrated in (this nice tool) https://github.com/zer0mem/MiniHyperVisorProject, to make it working on a live Windows (bluePill+Intercept R/W MSR+frookSINATRA = Rootkit ;p)
	* Real syscall analysis...but this is a Poc...
</pre>
