#!/usr/bin/env bash

# Check if we're running from within the repository
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

# Check if we're in a git repository and have the dotfiles directory
if git -C "$REPO_ROOT" rev-parse --git-dir > /dev/null 2>&1 && [ -d "$REPO_ROOT/modules/dotfiles" ]; then
    echo "Detected existing repository checkout, using local files..."
    DOTFILES_DIR="$REPO_ROOT/modules/dotfiles"
    CLEANUP=false
else
    echo "Cloning repository to temporary directory..."
    CHECKOUT_DIR=$(mktemp -d)
    pushd $CHECKOUT_DIR > /dev/null
    git clone --filter=blob:none --no-checkout https://github.com/sleaux/personal.git
    pushd monorepo > /dev/null
    git sparse-checkout init --cone
    git sparse-checkout set modules/dotfiles
    git checkout main
    DOTFILES_DIR="$PWD/modules/dotfiles"
    CLEANUP=true
fi

echo "Installing dotfiles..."
mkdir -p $HOME/.config
cp -rf $DOTFILES_DIR/tmux $HOME
cp -rf $DOTFILES_DIR/nvim $HOME/.config
cp -rf $DOTFILES_DIR/ghostty $HOME

if [ "$CLEANUP" = true ]; then
    popd > /dev/null
    popd > /dev/null
    rm -rf $CHECKOUT_DIR
    echo "Cleanup complete."
fi

echo "Dotfiles installation complete!"
