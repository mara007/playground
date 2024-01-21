#!/usr/bin/env bash
#

echo "install oh-my-zsh .."
sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

echo "install zsh-history-substring-search .."
git clone https://github.com/zsh-users/zsh-history-substring-search ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-history-substring-search

echo "install zsh-syntax-highlighting .."
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting

cat << EOF
put these to .zshrc
bindkey '^[[A' history-substring-search-up
bindkey '^[[B' history-substring-search-down

plugins=(autojump fzf zsh-autosuggestions zsh-history-substring-search zsh-syntax-highlighting )
EOF
