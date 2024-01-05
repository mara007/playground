#!/usr/bin/env bash
#

echo 'setup neovim + c++'
#
# ubuntu ppa (prefered)
echo install neovim from ppa + clang
#sudo add-apt-repository ppa:neovim-ppa/unstable
#sudo apt update
#sudo apt install clang neovim
# or get deb for old debian
# wget https://github.com/neovim/neovim-releases/releases/download/nightly/nvim-linux64.deb

echo get nerd fonts:
echo e.g. https://github.com/ryanoasis/nerd-fonts/releases/download/v3.1.1/JetBrainsMono.zip
echo 'and put it to ~/.local/share/fonts/ (or system /usr/share/fonts/)'
echo and setup your terminal to use it
read -p '<hit enter to continue>'

echo CLONE nvChad
git clone https://github.com/NvChad/NvChad ~/.config/nvim --depth 1

echo VIM config to nVim
echo 'vim.cmd("source ~/.vimrc")' >> ~/.config/nvim/init.lua

echo starting neovim, answer \'N\'o..
read -p '<hit enter to start neovim>'
nvim

echo PLUGINS to chadrc
cat > ~/.config/nvim/lua/custom/chadrc.lua << CHADRC
---@type ChadrcConfig
local M = {}

M.ui = {
    theme = 'catppuccin',
    tabufline = {
        show_numbers = true
    },
    hl_override = {
        Normal = {
            bg = "NONE"
        },
        -- NormalFloat = {
        --     bg = "NONE"
        -- }
    }
}

M.plugins = "custom.plugins"
M.mappings = require("custom.mappings")

return M
CHADRC


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
            }
        }
    }
}

return plugins
PLUGINS

echo LSP config
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

LSPCONFIG

echo starting neovim, type :MasonInstallAll
echo starting neovim, type :TSInstall cpp bash c make python vimdoc ini json
read -p '<hit enter to start neovim>'
nvim


