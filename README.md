# revoltCS
cs:go external in c++ dynamically acquiring offsets from y3's dumper

OffsetManager   => y3's dump
SettingsManager => custom settings at launch
csgo.h          => simple base classes (localPlayer, glowObject, Player, vec3D, ranks) 
                => and cs:go specific functions (aim calc, mouseAim, scanRanks)
main.cpp        => creation of threads, listen for user input, display menu
process.h       => custom slim memory reading/writing library
settings.h      => loads settings and offsets
slimstd.h       => small funcitons attempting to remove dependencies (a lot to be added) (q3sqrt is awesome)
