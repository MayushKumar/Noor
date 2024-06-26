;;; Directory Local Variables            -*- no-byte-compile: t -*-
;;; For more information see (info "(emacs) Directory Variables")

((nil . ((dape-configs . ((sandbox-codelldb modes (c-mode c-ts-mode c++-mode c++-ts-mode)
			  command-args ("--port" :autoport) ensure
			  dape-ensure-command command-cwd dape-command-cwd command
			  "~/.config/emacs/debug-adapters/codelldb/extension/adapter/codelldb"
			  port :autoport :type "lldb" :request "launch" :cwd "sandbox/"
			  :program "build/sandbox/Debug/sandbox" :args [] :stopOnEntry nil))))))
