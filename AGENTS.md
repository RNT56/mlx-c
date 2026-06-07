# Agent Entry Point

Read [CLAUDE.md](CLAUDE.md) first. It contains the workspace-level MLX fork
guide: multi-repo structure, TurboQuant goals, validation expectations,
benchmark rules, promotion gates, and handoff practices.

This repo is the C API bridge. Changes here usually expose lower-level MLX
runtime behavior to Swift or downstream callers and should be coordinated with
`mlx-swift` and `mlx-swift-lm`.
