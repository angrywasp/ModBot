# ModBot
Hardware and firmware for a modular robotics system

## Requirements 

PropGCC: https://github.com/angrywasp/propeller-gcc-windows  
Get two command line tools:  
[BSI, the BasicScript interpreter](https://github/com/angrywasp/basicscript). Scripts are written in BasicScript. Follow the link to learn more
PropTool. A basic command line tool for programming the Propeller chip. Compatible with official and unofficial propeller programming hardware
```
dotnet tool install -g AngryWasp.BasicScript.Interpreter
dotnet tool install -g AngryWasp.PropTool
```

Then build the framework
```powershell
bsi ./make.bs --% --run clean()
bsi ./make.bs --% --run buildFramework()
```

Hardware Notes: mn     mn     
