; Init:
G28 X Y ; home horizontally
G28 Z ; home vertically
M104 S215 ;set head temp (no wait)
M190 S60 ;set bed temp (wait)
M109 S215 ;set head temp (wait)
; Layer[0]:
M106 S0 ;turn off fan


   
G21 ; mm
G92 E0 ; reset extr
G90 ; abs
G0 F300 Z0.300

G1 X100.0 Y100.0
G1 X100.0 Y-100.0
G1 X-100.0 Y-100.0
G1 X-100.0 Y100.0


G0X0Y0
M104 S0 ;turn off head heater
M140 S0 ;turn off bed heater
M106 S0