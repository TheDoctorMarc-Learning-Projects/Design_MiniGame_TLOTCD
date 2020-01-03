@@@ TLOTCD Mini-Game AI Test
### CONTROLS
>>> Press 0 to toggle between Player vs IA and IA vs IA
>>> Press L to level up (max 3 times) 
>>> Press 1,2,3,4 to choose and attack (Player vs IA)
>>> Press 1,2 to choose a defence (Player vs IA) 
>>> Press WASD to change Character 1 strategy (IA vs IA):
1) WS toggles attack strategy
2) AD toggles defence strategy
>>> Press UP, DOWN, LEFT, RIGHT to change Character 2 strategy (both modes):
1) UP, DOWN toggles attack strategy
2) LEFT, RIGHT toggles defence strategy

### GAME
>>> Keep in mind when a character dies, another one spawns, so it 
can be the same one (check win label).

### AI 
>>> Attack:
1) Random Strategy is the default.
2) Weak Attack attacks when enough charge
3) Strong Attack does the same but with the strong attack
4) Charge fills the charge until starting point (10), then strong attack
5) Heal always heals unless max HP, then weak attack

>> Defence
1) Random Strategy is the default
2) Block always absorbs some of the damage
3) Evade can either ignore the damage or not absorb any of it 