fn main() {
    println!("cargo:rerun-if-changed=../../src/");
    
    cc::Build::new()
        .file("../../src/lexer.c")
        .file("../../src/parser.c")
        .file("../../src/value.c")
        .file("../../src/serializer.c")
        .file("../../src/error.c")
        .file("../../src/pxcf.c")
        .include("../../include")
        .include("../../src")
        .flag_if_supported("-std=c11")
        .compile("pxcf");
}
