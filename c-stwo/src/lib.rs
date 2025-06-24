#![allow(clippy::not_unsafe_ptr_arg_deref)]

use cairo_air::{PreProcessedTraceVariant, verifier::verify_cairo};

use stwo_cairo_prover::stwo_prover::core::{
    pcs::PcsConfig, vcs::blake2_merkle::Blake2sMerkleChannel,
};

#[cxx::bridge]
mod ffi {

    extern "Rust" {
        fn verify_cairo_proof(proof: &[u8], with_pedersen: bool) -> String;
    }
}

fn verify_cairo_proof(proof: &[u8], with_pedersen: bool) -> String {
    let proof = match serde_json::from_slice(proof) {
        Ok(proof) => proof,
        Err(e) => {
            println!("Error: {}", e);
            return "Invalid proof format".to_string();
        }
    };
    match verify_cairo::<Blake2sMerkleChannel>(
        proof,
        PcsConfig::default(),
        if with_pedersen {
            PreProcessedTraceVariant::Canonical
        } else {
            PreProcessedTraceVariant::CanonicalWithoutPedersen
        },
    ) {
        Ok(_) => "".to_string(),
        Err(e) => e.to_string(),
    }
}
