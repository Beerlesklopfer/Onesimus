fn main() -> Result<(), Box<dyn std::error::Error>> {
    let proto_dir = "../../proto";

    tonic_build::configure()
        .build_server(true)
        .build_client(true)
        .compile_protos(
            &[
                &format!("{proto_dir}/director_service.proto"),
                &format!("{proto_dir}/director_commands.proto"),
                &format!("{proto_dir}/plugin_ccc.proto"),
            ],
            &[proto_dir],
        )?;

    // Rebuild if protos change
    println!("cargo:rerun-if-changed={proto_dir}/director_service.proto");
    println!("cargo:rerun-if-changed={proto_dir}/director_commands.proto");
    println!("cargo:rerun-if-changed={proto_dir}/bareos_options.proto");
    println!("cargo:rerun-if-changed={proto_dir}/plugin_ccc.proto");

    Ok(())
}
