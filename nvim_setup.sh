#!/usr/bin/env bash
#

echo 'setup neovim + c++'
#
#
sudo add-apt-repository ppa:neovim-ppa/unstable
sudo apt update
sudo apt install clang nvim

echo CLONE nvChad
git clone https://github.com/NvChad/NvChad ~/.config/nvim --depth 1

echo VIM config to nVim
echo 'vim.cmd("source ~/.vimrc")' >> ~/.config/nvim/init.lua

echo PLUGINS to chadrc
cat > ~/config/nvim/lua/cusom/chadrc.lua << CHADRC
---@type ChadrcConfig
local M = {}

M.ui = { theme = 'catppuccin' }
M.plugins = "custom.plugins"

return M
CHADRC

echo PLUGINS config
mkdir ~/.config/nvim/lua/custom/
cat > ~/.config/nvim/lua/custom/plugins.lua << PLUGINS
local plugins = {
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
        "clangd"
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


