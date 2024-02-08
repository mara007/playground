#!/usr/bin/env bash
#
cat << DEPS
################################################################
# Dependecies:
# * sudo apt install zsh bat ripgrep lsd nvim
#  - ..if you don't like those, remove its alias
#
# * terminal with nerdfont
#  - e.g. https://github.com/ryanoasis/nerd-fonts/releases/download/v3.1.1/JetBrainsMono.zip
#    .. and put it to ~/.local/share/fonts/ (or system /usr/share/fonts/)
################################################################
.. press <enter> to continue
DEPS
read

echo '################################################################'
echo "install oh-my-zsh .. (ctrl+d after)"
sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
echo '################################################################'
echo

echo "install zsh-history-substring-search .."
git clone https://github.com/zsh-users/zsh-history-substring-search ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-history-substring-search

echo "install zsh-syntax-highlighting .."
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting

echo "install zsh-autosuggestions .."
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions


cat << EOF
update plugins to: these to .zshrc

plugins=(autojump fzf zsh-autosuggestions zsh-history-substring-search zsh-syntax-highlighting )
EOF

echo " updating ~/.zshrc .."
cat >> ~/.zshrc << MY_ZSHRC
###########################
#      MY MY MY           #
#

bindkey '^[[A' history-substring-search-up
bindkey '^[[B' history-substring-search-down

# better ssh completion
autoload -Uz compinit
compinit

alias vim=nvim
alias vi='nvim --clean'
alias bat=batcat
alias cat=batcat
alias ls=lsd

EOF


cat << EOF
Favorite themes:
kafeitu
nebirhos
michelebologna
evan
EOF
