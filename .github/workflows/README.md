# GitHub Actions Workflows

This directory contains CI/CD workflows for the BIT-S project.

## Workflows

### `ci.yml`
- Triggers on changes to `proto/` directory
- Generates Protobuf bindings for C++ and Rust
- Compiles both codebases to ensure compatibility
- Auto-commits generated files on main branch

### `docker-build.yml`
- Builds Docker images on push to main
- Tests Docker Compose setup
- Ensures services can start and communicate

## Usage

Workflows run automatically on:
- Push to `main` or `develop` branches
- Pull requests targeting `main` or `develop`
- Changes to `proto/` directory

Manual triggers can be added via GitHub Actions UI.
