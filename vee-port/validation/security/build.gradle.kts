import com.microej.gradle.plugins.TestMode
import com.microej.gradle.plugins.TestTarget

plugins {
    id("com.microej.gradle.testsuite")
}

microej {
    architectureUsage = System.getProperty("com.microej.architecture.usage") ?: "eval" // or "prod"
    skippedCheckers = "changelog,readme,license"
}

dependencies {
    microejVee(project(":vee-port"))
}

testing {
    suites {
        val test by getting(JvmTestSuite::class) {
            microej.useMicroejTestEngine(this, TestTarget.EMB, TestMode.MAIN)

            dependencies {
                implementation(project())
                implementation(libs.junit)
                implementation(libs.junit.platform)

                implementation(libs.api.edc)
                implementation(libs.api.bon)
                implementation(libs.testsuite.security)
                implementation(libs.api.security)
            }

            targets {
                all {
                    testTask.configure {
                        filter {
                            excludeTestsMatching("*AllTestClasses")
                            excludeTestsMatching("*SingleTest*")
                            // do not embed inner classes as test classes
                            excludeTestsMatching("*$*")

                            // _AllTests_CertificateDataTest is not appropriate for mbedtls platforms
                            excludeTestsMatching("*CertificateDataTest*")
                        }
                    }
                }
            }
        }
    }
}