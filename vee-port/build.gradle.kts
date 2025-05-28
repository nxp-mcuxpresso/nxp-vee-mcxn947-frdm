plugins {
    id("com.microej.gradle.veeport") version libs.versions.microej.sdk
}

microej {
    skippedCheckers = "changelog,readme,license"
}

dependencies {
    microejArchitecture(libs.architecture)

    microejPack(libs.pack.ui.architecture)
    microejPack(libs.pack.ai)
    microejPack(libs.pack.device)
    microejPack(libs.pack.event.queue)
    microejPack(libs.pack.fs)
    microejPack(libs.pack.gpio)
    microejPack(libs.pack.gpio.properties)
    microejPack(libs.pack.net)
    microejPack(libs.pack.net.addons)


    microejFrontPanel(project(":vee-port:front-panel"))

    microejMock(project(":vee-port:mock"))
}
