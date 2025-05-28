rootProject.name = "nxpvee-mcxn947-frdm"
include("vee-port", "vee-port:front-panel", "vee-port:mock")
include("apps:aiSample", "apps:simpleGFX")

project(":vee-port:front-panel").projectDir = file("vee-port/extensions/front-panel")
project(":vee-port:mock").projectDir = file("vee-port/mock")

include("vee-port:validation:core")
include("vee-port:validation:event-queue")
include("vee-port:validation:fs")
include("vee-port:validation:gpio")
include("vee-port:validation:net")
include("vee-port:validation:security")
include("vee-port:validation:ssl")