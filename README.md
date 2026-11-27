# CITA417FINAL
## Lab 10: Autonomous AI & Systems Integration
```
Player Controller → ACITA417FINALCharacter ◄── possesses ── AEnemyAIController
                      │ (same class)                          ▲        │
                      ├─ WeaponComponent → Trace → ApplyPointDamage → HealthComponent → Death
                      └─ Speed / Direction / Airborne / Attacking → AnimInstance → ABP_Character

AI Perception (Sight) → AEnemyAIController → Blackboard ⇄ Behavior Tree
   (Selector/Sequence, Decorators, UpdateCombatState service) → C++ Tasks (Attack, FindPatrolPoint) → Movement / WeaponComponent
```

**Decision design:** Perception writes `TargetActor` and `LastKnownLocation` into the Blackboard. The Behavior Tree chooses between Combat, Search, and Patrol. Tasks reuse the existing movement, weapon, health, and animation systems.

**Differs from a tutorial:** the AI uses the same Character, WeaponComponent, and HealthComponent as the player. The search timeout, attack range, and patrol radius are editable.

**Independent improvement:** configurable Attack Range (BT service), Search Timeout (controller), and Patrol Radius (task), which together allow multiple enemy archetypes from the same framework.

## Lab 09: Combat & Damage Architecture
```
Input (IA_Fire) → Character → WeaponComponent → Line Trace → ApplyPointDamage → HealthComponent → Feedback / Death
```

**Crate question:** If I add a destructible crate tomorrow, the input, the Character, the WeaponComponent, the line trace, and the HealthComponent all work unchanged. The crate only needs a mesh with trace collision and a HealthComponent. The weapon never needs to know what a crate is, because it only applies damage to whatever Actor it hits.

**Independent improvement:** Fire cooldown (rate of fire) in WeaponComponent, plus an upper-body attack layer driven by an `IsAttacking` fact exposed through the Lab 08 AnimInstance.

# CITA417FINAL – Lab 08: Character State \& Animation Architecture



## Architecture

```

Gameplay / Character State  →  Animation Blueprint / State Machine / Blend Space  →  Final Skeletal Pose  →  Rendering

(ACITA417FINALCharacter:       (UCITA417AnimInstance reads state;

&#x20;Speed, Direction, Airborne)    ABP\_Character chooses and blends clips)

```



**Responsibilities:** Gameplay owns facts such as speed, movement direction, and airborne status. Animation owns which clip plays, how clips blend, when transitions happen, and the final pose. If many gameplay functions called individual animations directly, every gameplay change would require touching animation logic, and animation changes would require editing gameplay code. That coupling makes the code hard to test and reuse, and it breaks when new characters or states are added. Here, gameplay exposes read-only state and the Animation Blueprint decides how to show it, so either side can change independently.



## Debug evidence

On-screen text in `NativeUpdateAnimation` shows Speed, Direction, and Airborne. They are produced by the Character getters and consumed by `UCITA417AnimInstance` and `ABP\_Character`.



## Independent improvement

Landing state. The Animation Blueprint enters Land when `bIsAirborne` goes false, so no new gameplay code is needed. It shows that new animation behavior can be added purely in the animation layer.

