# FlameRobin fork — Windows handover notes

This document tracks fixes made on the macOS side that need verification or
follow-up on a Windows build environment. Pick up by checking each section
in order.

## Branch

All work lives on `andre/master` (https://github.com/andrevanzuydam/flamerobin).
Pull and build with vcpkg disabled or enabled — same source.

```
git checkout master
git pull andre master
git submodule update --init --recursive
```

## Build prerequisites (Windows)

The fork synced with upstream `mariuz/master @ 4e0b55f9`, which introduced
the `vcpkg` and `src/fb-cpp` submodules and the modern `fb-cpp` engine
backend (now the default). On Windows the vcpkg path provides a self-
contained build of Firebird, Lexilla, Scintilla, and dependencies.

```
cmake -B build -S . -DENABLE_VCPKG=ON
cmake --build build --config Release
```

If vcpkg is too heavy for a quick rebuild, set `-DENABLE_VCPKG=OFF` and
provide wxWidgets / Firebird headers via system installs.

## Fixes already applied on this branch

Each is a single commit titled `Fix #NNN: ...` so they cherry-pick
cleanly to upstream when the moratorium lifts.

| Issue | Status on macOS | Windows verification needed |
|---|---|---|
| `#550` orange editor background | fixed (cherry-pick `2d631802`, `51af48ec`) | confirm style configurator save → editor stays correct color in **light AND dark** mode on Windows |
| `#575` missing keyword highlight | fixed (`705c3b8b`) | confirm BOOLEAN / TRUE / FALSE / RETURN / DETERMINISTIC highlight before connecting to a database |
| `#296` window stays maximised | fixed (`88b43191`) | open Preferences → Visual style → tick "Don't restore maximised state"; verify properties windows reopen at default size |
| `#409` "Dependencies broken" false warning | fixed (`a6083b49`) | run the issue's repro script (CREATE PROC + CREATE VIEW + ALTER PROC) on FB4+; warning should NOT appear |
| `#378` properties pane stuck on "Please wait" | fixed (`d2f5de9e`) | trigger any failing template (e.g. against an FB1 server if you have one) and confirm an error page renders instead of perpetual loading |
| `#355` 32-char field truncation | fixed for fb-cpp default (`e201cfea` adds doc note) | with the default fb-cpp backend, run a SELECT against a column with a 33+ char name on FB4+; full name should display. If user switched to IBPP backend they still get truncation — this is by design until OO API integration in IBPP |
| `#301` DECIMAL → INTEGER for SP parameter | fixed (`6fa7cf4e`) | create a stored procedure with a DECIMAL(18,0) parameter; verify the regenerated DDL keeps DECIMAL(18,0), not INTEGER |
| `#239` filter box for tree | **in progress** — see "Open work" below | n/a until implemented |
| `#379` embedded DB lock | **deferred — needs Windows env** | see "Deferred fixes" below |

## Open work — pick up here on Windows

### `#239` — Add filter to reduce server/db list at main window

Status: not yet implemented. Plan:

1. Add a `wxTextCtrl* filterBoxM` member to `MainFrame.h` (with a clear-X
   button next to it) and a wxPanel container `filterPanelM`.
2. In `MainFrame::do_layout`, prepend `filterPanelM` to `searchPanelSizerM`
   above the existing tree (so layout becomes: filter / tree / search).
3. Bind `EVT_TEXT` on the filter to a new `OnFilterTextChange` handler
   that walks the tree (`treeMainM`):
   - If filter empty → call `Expand`/`Collapse` to restore prior state
     (or just leave whatever the user had).
   - Otherwise: for each tree item, recursively check if the item's
     label (case-insensitive) contains the filter text, OR any
     descendant matches. Items with no match in their subtree get
     collapsed; items with matches get expanded so the matches are
     visible. Use `wxTreeCtrl::SetItemBold` or
     `SetItemBackgroundColour` to highlight actual matches.
4. Persist filter visibility under a `showTreeFilter` config flag.
5. Add a "View → Show tree filter" menu toggle to mirror the existing
   search bar toggle.

Per-platform note: wxTreeCtrl on Windows supports `Hide()` of items
through the underlying SysTreeView32 control via `TVM_DELETEITEM` +
re-insert. To avoid the round-trip cost on a 50-server tree, prefer the
expand/collapse + highlight approach above.

### `#379` — Embedded DB file lock

Status: deferred. The reporter overwrote a connected embedded `.fdb` file
with `cp` (Windows Explorer copy) and corrupted the database. Needed: a
Windows file lock that prevents copy-over while connected.

Approach to evaluate on Windows:

1. After `Database::Connect` succeeds for an embedded connection, open
   a Windows `HANDLE` via `CreateFileW(path, GENERIC_READ,
   FILE_SHARE_READ | FILE_SHARE_WRITE /* allow FB to keep writing */,
   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)`.
2. Critically do **not** include `FILE_SHARE_DELETE` — this prevents
   another process from opening the file with `DELETE` access, which
   is what `CopyFile` and Explorer's overwrite use.
3. Hold the handle in `DatabaseImpl` for the lifetime of the connection.
4. Test: with the connection live, attempt to copy another file over
   the `.fdb` from another shell — it should fail with sharing violation.
5. Test: make sure normal Firebird operations still work (the engine
   has its own `HANDLE` to the same file with its own sharing flags;
   `FILE_SHARE_READ | FILE_SHARE_WRITE` from our side should be
   compatible).
6. Detect "embedded" vs server connection: if `mServerName.empty()`
   (already used in `database.cpp:Connect`) treat as embedded.

Risk to verify before merging: some Windows AV / backup software opens
files with `FILE_SHARE_DELETE`; refusing this share could surface
spurious sharing-violation errors during normal antivirus scans.

### Reported but not filed: Grid edit hang on Windows

User reports that editing 3 rows in a query-result grid then editing a
4th cell freezes the app on Windows (works fine on macOS).

Diagnosis steps to do on Windows:

1. Reproduce: open a SELECT result, edit cells in 3 different rows
   pressing Enter to commit each. Try to edit a 4th cell.
2. When frozen, capture a stack trace:
   - In a Visual Studio debugger: attach to the running flamerobin.exe,
     pause, look at all threads' call stacks.
   - Or `procdump -ma -h <pid> dump.dmp` and analyse offline.
3. Look for: blocking on a hidden modal (the most common Windows-only
   freeze pattern when macOS shows the app as idle); blocking on
   `isc_dsql_execute2` (lock contention on the SELECT cursor); blocking
   on a `SubjectLocker` deadlock.
4. Suspect commits to look at first:
   - Upstream `30f459fa Fix IBPP backend crash: return 0 instead of
     throwing exception in getColumnCount...` — already merged into our
     fork; may have fixed this. Test first whether the merged build
     still reproduces.
   - Our `pr/fix-390-slow-grid-context-menu` change in
     `OnGridUpdateMenuRemoveRow` — check if `tb->canRemoveRow(0)`
     misbehaves on Windows when called repeatedly during a paint.

Look in `src/gui/controls/DataGridTable.cpp` `SetValue` and
`src/gui/controls/DataGridRows.cpp` `setFieldValue` — both run on the
main thread, so any blocking IBPP call freezes the UI.

## Release process (mac side, for reference)

When new fixes have been validated on Windows and are ready to ship:

```
# Ensure you're on master and clean
cd /Users/andrevanzuydam/IdeaProjects/flamerobin
git status

# Run the signed + notarised release
./dist/macos/release.sh

# The script auto-derives version from src/frversion.h, signs with the
# Code Infinity Developer ID, submits to Apple notary, staples the
# ticket, and produces:
#   build-release/Release/flamerobin.app
#   build-release/dist/FlameRobin-<version>-macos-arm64.zip

# Tag and publish on the fork:
git tag -a v<version>-macos-arm64 -m "..."
git push andre v<version>-macos-arm64
gh release create v<version>-macos-arm64 ... build-release/dist/...zip
```

## Upstream PR moratorium

User has paused upstream PRs for now. When the moratorium lifts, the
per-issue commits cherry-pick cleanly. Suggested grouping:

- One PR per issue (e.g. `pr/fix-575-keyword-set-default`,
  `pr/fix-296-no-restore-maximised`, etc.).
- Or one combined PR titled "Batch of small fixes" with a clear list of
  what each commit addresses — easier review for upstream.
