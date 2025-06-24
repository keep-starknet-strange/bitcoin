fn main() {
    cxx_build::bridge("src/lib.rs")
        .flag_if_supported("-std=c++17")
        .include("include") // Header output location
        .compile("c_stwo");

    println!("cargo:rerun-if-changed=src/lib.rs");
}
