# CITA417FINAL



\# CITA417FINAL – Lab 08: Character State \& Animation Architecture



\## Architecture

```

Gameplay / Character State  →  Animation Blueprint / State Machine / Blend Space  →  Final Skeletal Pose  →  Rendering

(ACITA417FINALCharacter:       (UCITA417AnimInstance reads state;

&#x20;Speed, Direction, Airborne)    ABP\_Character chooses and blends clips)

```



\*\*Responsibilities:\*\* Gameplay owns facts such as speed, movement direction, and airborne status. Animation owns which clip plays, how clips blend, when transitions happen, and the final pose. If many gameplay functions called individual animations directly, every gameplay change would require touching animation logic, and animation changes would require editing gameplay code. That coupling makes the code hard to test and reuse, and it breaks when new characters or states are added. Here, gameplay exposes read-only state and the Animation Blueprint decides how to show it, so either side can change independently.



\## Debug evidence

On-screen text in `NativeUpdateAnimation` shows Speed, Direction, and Airborne. They are produced by the Character getters and consumed by `UCITA417AnimInstance` and `ABP\_Character`.



\## Independent improvement

Landing state. The Animation Blueprint enters Land when `bIsAirborne` goes false, so no new gameplay code is needed. It shows that new animation behavior can be added purely in the animation layer.

