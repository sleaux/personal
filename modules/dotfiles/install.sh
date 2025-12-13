#!/usr/bin/env bash

CHECKOUT_DIR=$(mktemp -d)
pushd $CHECKOUT_DIR
git clone --filter=blob:none --no-checkout https://github.com/sleaux/monorepo.git
pushd monorepo
git sparse-checkout init --cone
git sparse-checkout set modules/dotfiles
git checkout main

mkdir -p $HOME/.config
cp -rf modules/dotfiles/tmux $HOME
cp -rf modules/dotfiles/nvim $HOME/.config
cp -rf modules/dotfiles/ghostty $HOME

popd
popd

rm -rf $CHECKOUT_DIR
