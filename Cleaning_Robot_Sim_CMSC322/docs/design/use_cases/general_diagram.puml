@startuml
left to right direction
/'actors'/
"building manager" as bm
"maintenance" as m
"robot" as r

/'check actions'/
"check availability" as (ca)
"check status" as (cs)
"check location" as (cl)

/'other actions'/
"assign tasks" as (at)
"execute tasks" as (ex)
"go back home" as (gh)

bm --> (ca): find idle robot(s) to do cleaning
bm --> (at): ask a robot to execute a specific cleaning task

m --> (cs): check a robot's status to see whether it is 'fault'
m --> (cl): check a 'fault' robot's location to pick it up

r --> (ex): execute the task assigned by bm
r --> (gh): finish a task and go back to the hub

@enduml