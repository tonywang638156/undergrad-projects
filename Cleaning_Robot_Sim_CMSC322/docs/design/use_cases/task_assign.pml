@startuml
left to right direction
/'actors'/
"building manager" as bm
"mopping robot" as mopr
"scrubbing robot" as scrubr
"vacuuming robot" as vacuumr

/'check actions'/
"scrub room" as (sr)
"mop room" as (mr)
"vacuum room" as (vr)

bm --> (sr): assign robot to scrub room
bm --> (mr): assign robot to mop room
bm --> (vr): assign robot to vacuum room

(sr) --> scrubr: assign robot to scrub room
(mr) --> mopr: assign robot to mop room
(vr) --> vacuumr: assign robot to vacuum room
@enduml