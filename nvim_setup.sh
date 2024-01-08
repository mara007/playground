#!/usr/bin/env bash
#

echo 'setup neovim + c++'
#
# ubuntu ppa (prefered)
echo install neovim from ppa
sudo add-apt-repository ppa:neovim-ppa/unstable
#optional - old debian: apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 55F96FCF8231B6DD
sudo apt update
sudo apt install neovim
# or get deb for old debian
# wget https://github.com/neovim/neovim-releases/releases/download/nightly/nvim-linux64.deb

echo
echo 'install dependecies (clang, ..)'
sudo apt install clang npm python3-venv ripgrep

echo
echo '!!! get nerd fonts !!!'
echo e.g. https://github.com/ryanoasis/nerd-fonts/releases/download/v3.1.1/JetBrainsMono.zip
echo 'and put it to ~/.local/share/fonts/ (or system /usr/share/fonts/)'
echo and setup your terminal to use it
read -p '<hit enter to continue>'

echo
echo CLONE nvChad
git clone https://github.com/NvChad/NvChad ~/.config/nvim --depth 1

echo
echo VIM config to nVim
cat >> ~/.config/nvim/init.lua << VIMRC
vim.cmd("source ~/.vimrc")
vim.opt.whichwrap= nil
vim.opt.swapfile = false
vim.opt.list = true
-- overwriten somewhere vim.opt.listchars = "tab:▸\\ ,trail:·"

VIMRC

echo
echo starting neovim, answer \'N\'o..
read -p '<hit enter to start neovim>'
nvim

echo
echo PLUGINS to chadrc
cat > ~/.config/nvim/lua/custom/chadrc.lua << CHADRC
---@type ChadrcConfig
local M = {}

M.ui = {
    theme = 'catppuccin',
    theme_toggle = { "catppuccin", "ayu_light" },
    tabufline = {
        show_numbers = true
    },
    -- marek - enable transparent background
    -- hl_override = {
    --     Normal = {
    --         bg = "NONE"
    --     },
    -- }
}

M.plugins = "custom.plugins"
M.mappings = require("custom.mappings")

-- marek - better vimtree
require("custom.configs.nvimtree")

return M
CHADRC


echo
echo MAPPINGS for debug
cat > ~/.config/nvim/lua/custom/mappings.lua << MAPPINGS
local M = {}

M.dap = {
    plugin = true,
    n = {
        ["<leader>db"] = {
            "<cmd> DapToggleBreakpoint <CR>",
            "Add breakpoint at line",
        },   
        ["<leader>dr"] = {
            "<cmd> DapContinue <CR>",
            "Start or continue the debugger",
        },
        -- moje
        ["<C-p>"] = { "<cmd> Telescope find_files <CR>", "Find files", },
    }
}

return M
MAPPINGS
		        	sdf

echo
echo PLUGINS config
mkdir ~/.config/nvim/lua/custom/
cat > ~/.config/nvim/lua/custom/plugins.lua << PLUGINS
local plugins = {
    {
        "rcarriga/nvim-dap-ui",
        event = "VeryLazy",
        dependencies = "mfussenegger/nvim-dap",
        config = function()
            local dap = require("dap")
            local dapui = require("dapui")
            dapui.setup()
            dap.listeners.after.event_initialized["dapui_config"] = function()
                dapui.open()
            end
            dap.listeners.before.event_terminated["dapui_config"] = function()
                dapui.close()
            end
            dap.listeners.before.event_exited["dapui_config"] = function()
                dapui.close()
            end
        end
    },
    {
        "jay-babu/mason-nvim-dap.nvim",
        event = "VeryLazy",
        dependecies = {
            "williamboman/mason.nvim",
            "mfussenegger/nvim-dap",
        },
        opts = {
            handlers = {},
        },
    },
    {
        "mfussenegger/nvim-dap",
        config = function(_, _)
            require("core.utils").load_mappings("dap")
        end
    },
    {
        "neovim/nvim-lspconfig",
        config = function()
            require "plugins.configs.lspconfig"
            require "custom.configs.lspconfig"
        end,
    },
    {
        "williamboman/mason.nvim",
        opts = {
            ensure_installed = {
                "clangd",
                -- "clang-format",
                "codelldb",
                "pyright",
            }
        }
    },
    {
        -- marek - workaround - editing lua files
        "nvim-treesitter/nvim-treesitter",
        lazy = false,
    }

}

return plugins
PLUGINS

echo
echo 'LSP (Language Server Protocol) config'
mkdir ~/.config/nvim/lua/custom/configs/
cat > ~/.config/nvim/lua/custom/configs/lspconfig.lua << LSPCONFIG
local base = require("plugins.configs.lspconfig")
local on_attach = base.on_attach
local capabilities = base.capabilities

local lspconfig = require("lspconfig")

lspconfig.clangd.setup {
  on_attach = function (client, bufnr)
    client.server_capabilities.signatureHelpProvider = false
    on_attach(client,bufnr)
  end,
  capabilities = capabilities,
}

lspconfig.pyright.setup({
    on_attach = on_attach,
    capabilities = capabilities,
    filetypes = {"python"},
})

LSPCONFIG


echo
echo 'nvimtree config'
cat > ~/.config/nvim/lua/custom/configs/nvimtree.lua << NVIMTREE
local nvim_tree_conf = require("plugins.configs.nvimtree")

nvim_tree_conf.renderer.highlight_opened_files = "all"
nvim_tree_conf.renderer.root_folder_label = true
nvim_tree_conf.git.enable = true
nvim_tree_conf.renderer.icons.show.git = true

NVIMTREE

cat << FINAL_TODOS

starting neovim, do these commands!
================================================================================
to install syntax higlighters type (theres much more, check :TSInstall <tab>)

:TSInstall cpp bash c make python vimdoc vim ini json markdown


================================================================================
(if there is Lazy.vim setup window opened, pres "q" before invoking Mason)
to update tools - type

:MasonInstallAll

================================================================================
basic keybinds (normal mode) (space=leader):
    ctrl+n - directory tree
    shift+k - info about item under cursor (class definition, etc)
    alt+h - terminal (ctrl+x to deselect)
    <space>ch - keybinds cheat sheet (CHECK THIS)
    <space>th - themes
FINAL_TODOS
read -p '<hit enter to start neovim>'
nvim

