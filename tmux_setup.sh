#!/usr/bin/env bash
#

echo setup tmux tpm

git clone https://github.com/tmux-plugins/tpm ~/.tmux/plugins/tpm

echo setup tmux conf

cat > ~/.my_tmux_conf << CONF
# set colors
# set-option -sa terminal-overrides ",xterm*:Tc"
set -ag terminal-overrides ",*:RGB"

# List of plugins
set -g @plugin 'tmux-plugins/tpm'
set -g @plugin 'tmux-plugins/tmux-yank'
set -g @plugin 'tmux-plugins/tmux-sensible'
set -g @plugin 'christoomey/vim-tmux-navigator'
# theme
set -g @plugin 'catppuccin/tmux'


set -g mouse on
set -g display-panes-time 2500
set-environment -g 'SSH_AUTH_SOCK' ~/.ssh/ssh_auth_sock

set -g allow-rename off
set-window-option -g allow-rename off
set -wg automatic-rename off

set -g @catppuccin_window_status_enable "yes"
set -g @catppuccin_window_status_icon_enable "yes"
set -g @catppuccin_window_status_style "slanted"

set -ggq @catppuccin_window_text "#W"
set -ggq @catppuccin_window_current_text "#W"


# Make the status line pretty and add some modules
set -g status-right-length 100
set -g status-left-length 100
set -g status-left ""
set -g status-right "#{E:@catppuccin_status_application}"
set -agF status-right "#{E:@catppuccin_status_cpu}"
set -ag status-right "#{E:@catppuccin_status_session}"
set -ag status-right "#{E:@catppuccin_status_uptime}"

# my binds
bind  c  new-window      -c "#{pane_current_path}"
bind  %  split-window -h -c "#{pane_current_path}"
bind '"' split-window -v -c "#{pane_current_path}"


# Initialize TMUX plugin manager (keep this line at the very bottom of tmux.conf)
run '~/.tmux/plugins/tpm/tpm'


CONF

if [ -e ~/.tmux.conf ] ; then
    echo adding original ~/.tmux.conf
    cat ~/.tmux.conf >> ~/.my_tmux_conf
fi
mv ~/.my_tmux_conf ~/.tmux.conf

echo run tmux and '<prefix>+I' to install plugins

