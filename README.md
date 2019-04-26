# MonoJabber - It's a Unity DLL Injector
This is an effort to produce my own DLL injector for usage with games using the Mono library.  
At the moment, it only supports modern Unity games (that is, games which use mono-2.0-bdwgc.dll).  

# Usage
Download the currently released version from the releases page. **Make sure MonoLoaderDLL.dll and MonoJabber.exe are in the same folder.** At the moment, **this tool has been written under & made with a 64 bit version of Windows in mind.**   
It does not support 32 bit processes.  

I've tested this extensively using a hack I wrote for SCP: Secret Laboratory, and I've had much success. If you find any bugs please do open an issue, or contact me.  

The program takes **5 arguments**: {TargetProcess} {Path to Payload DLL} {PayloadNamespace} {PayloadClassname} {PayloadMethodname}  

**EG:** `MonoJabber.exe "SCPSL.exe" "D:\Source\SCP-Exposed-Establishment\SCPEE\bin\x64\Release\SCPEE.dll" "SCPEE.NotEvil" "EELoader" "Init"`

# How it works
It was a massive pain in the neck digging up the required resources to create this tool.
Other Mono injectors I've seen all do things completely differently. [**SharpMonoInjector**](https://github.com/warbler/SharpMonoInjector#sharpmonoinjector) for example does really interesting voodoo with dynamically generating ASM and injecting it into the process.  

But, the end result of all mono injectors is the same: **getting into the target Mono application, and then calling native Mono functions to bootstrap and initialize the payload DLL.**  

The difference in the injector is just how they get to the whole "call native Mono functions" part. I think I struck a balance between voodoo and simplicity.  

How mine works is: the frontend for the tool, *MonoJabber.exe*, simply takes the target, then injects a DLL, 'MonoLoaderDLL'. It is *MonoLoaderDLL* which does all the heavy lifting.  
*MonoLoaderDLL* exports a single function: `Inject`, which takes one argument of type `void*`. 
 
In MonoJabber.exe, a struct with information passed to the program is written to the memory of the target, and MonoJabber calls `CreateRemoteThread` passing the address of the struct to the `Inject` function. Then the Inject function calls `LoadLibrary` on `mono-2.0-bdwgc.dll` and then begins calling relevant functions from that DLL using information from the argument struct.

# Resources Used
https://github.com/corngood/mono/blob/master/mono/metadata/image.h
http://docs.go-mono.com/index.aspx?link=xhtml%3Adeploy%2Fmono-api-class.html
http://docs.go-mono.com/index.aspx?link=xhtml%3Adeploy%2Fmono-api-domains.html
http://docs.go-mono.com/index.aspx?link=xhtml%3Adeploy%2Fmono-api-image.html
https://www.unknowncheats.me/forum/unity/268053-mono-unity-injector.html
https://www.unknowncheats.me/forum/general-programming-and-reversing/105244-accessing-c-mono.html

# License  
This library is licensed under version 3 of the General Public License. You are free to do whatever you wish the code in this library. No credit due.