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

# no error when parameter wildcard expansion has no match
unsetopt nomatch

alias vim=nvim
alias vi='nvim --clean'
alias bat=batcat
alias cat=batcat
alias ls=lsd

#REQUIRES BEAR
# build macro with generation of build commands for clangd
alias m='bear -- make -j15 2>&1|lolcat'

EOF


cat << EOF
Favorite themes:
================
af-magic (with git)
agnoster (with git)
apple (git)
arrow
crunch (with git)
cypher (no git)
eastwood
evan
evan
example
gallifrey (with git)
gozilla
jnrowe
kafeitu
kennethreitz
kennethreitz
lukerandall (git)
maran
michelebologna
minimal
muse (with git)
nebirhos
robbyrussell (with git)
sonicradish (with git)
sorin  (with git)
theunraveler (with git)
tonotdo
================
EOF

