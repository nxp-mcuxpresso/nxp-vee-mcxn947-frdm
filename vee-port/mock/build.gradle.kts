plugins {
    id("com.microej.gradle.mock")
}

microej {
    skippedCheckers = "changelog,readme,license,nullanalysis"
}

dependencies {
    implementation(files("lib/HILEngine.jar"))
}