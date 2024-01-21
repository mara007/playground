#!/usr/bin/env bash
#

echo setup tmux tpm

git clone https://github.com/tmux-plugins/tpm ~/.tmux/plugins/tpm

echo setup tmux conf

cat > ~/.my_tmux_conf << CONF
# set colors
set-option -sa terminal-overrides ",xterm*:Tc"
# List of plugins
set -g @plugin 'tmux-plugins/tpm'
set -g @plugin 'tmux-plugins/tmux-yank'
set -g @plugin 'tmux-plugins/tmux-sensible'
set -g @plugin 'christoomey/vim-tmux-navigator'
# theme
set -g @plugin 'catppuccin/tmux'


# Other examples:
# set -g @plugin 'github_username/plugin_name'
# set -g @plugin 'github_username/plugin_name#branch'
# set -g @plugin 'git@github.com:user/plugin'
# set -g @plugin 'git@bitbucket.com:user/plugin'

set -g @catppuccin_window_status_enable "yes"
set -g @catppuccin_window_status_icon_enable "yes"

set -g @catppuccin_window_status_enable "yes"
set -g @catppuccin_window_status_icon_enable "yes"
set -g @catppuccin_window_left_separator ""
set -g @catppuccin_window_right_separator " "
set -g @catppuccin_window_middle_separator " █"
set -g @catppuccin_window_number_position "right"
set -g @catppuccin_window_default_text "#W"
set -g @catppuccin_window_current_text "#W"

# set-option -g status-position top
#
# Initialize TMUX plugin manager (keep this line at the very bottom of tmux.conf)
run '~/.tmux/plugins/tpm/tpm'

CONF

if [ -e ~/.tmux.conf ] ; then
    echo adding original ~/.tmux.conf
    cat ~/.tmux.conf >> ~/.my_tmux_conf
    mv ~/.my_tmux_conf ~/.tmux.conf
fi

echo run tmux and '<prefix>+I' to install plugins

