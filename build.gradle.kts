plugins {
    kotlin("jvm") version "2.1.0-RC"
}

group = "fun.eternalblue"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(kotlin("test"))
    implementation("com.fazecast:jSerialComm:2.9.0")
    implementation("org.jetbrains.kotlinx:kotlinx-coroutines-core:1.9.0")
}

tasks.test {
    useJUnitPlatform()
}