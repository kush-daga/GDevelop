// @flow
import { Trans } from '@lingui/macro';
import { t } from '@lingui/macro';
import { I18n } from '@lingui/react';
import { type I18n as I18nType } from '@lingui/core';

import * as React from 'react';
import Toggle from '../../UI/Toggle';
import { mapFor } from '../../Utils/MapFor';
import EmptyMessage from '../../UI/EmptyMessage';
import ParameterRenderingService from '../ParameterRenderingService';
import HelpButton from '../../UI/HelpButton';
import {
  type ResourceSource,
  type ChooseResourceFunction,
} from '../../ResourcesList/ResourceSource.flow';
import { type ResourceExternalEditor } from '../../ResourcesList/ResourceExternalEditor.flow';
import { Line, Spacer } from '../../UI/Grid';
import AlertMessage from '../../UI/AlertMessage';
import { getExtraInstructionInformation } from '../../Hints';
import { isAnEventFunctionMetadata } from '../../EventsFunctionsExtensionsLoader';
import OpenInNew from '@material-ui/icons/OpenInNew';
import IconButton from '../../UI/IconButton';
import { type EventsScope } from '../EventsScope.flow';
import { getObjectParameterIndex } from './InstructionOrExpressionSelector/EnumerateInstructions';
import Text from '../../UI/Text';
import { getInstructionMetadata } from './NewInstructionEditor';
const gd = global.gd;

const styles = {
  // When displaying parameters, take all the height:
  container: {
    display: 'flex',
    flexDirection: 'column',
    flex: 1,
  },
  // When displaying the empty message, center the message:
  emptyContainer: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
  },
  parametersContainer: {
    flex: 1,
    overflowY: 'auto',
  },
  icon: {
    width: 24,
    height: 24,
    marginRight: 8,
    flexShrink: 0,
  },
  invertToggle: {
    marginTop: 8,
  },
};

type Props = {|
  project: gdProject,
  scope: EventsScope,
  globalObjectsContainer: gdObjectsContainer,
  objectsContainer: gdObjectsContainer,
  objectName?: ?string,
  instruction: gdInstruction,
  isCondition: boolean,
  focusOnMount?: boolean,
  resourceSources: Array<ResourceSource>,
  onChooseResource: ChooseResourceFunction,
  resourceExternalEditors: Array<ResourceExternalEditor>,
  style?: Object,
  openInstructionOrExpression: (
    extension: gdPlatformExtension,
    type: string
  ) => void,
  noHelpButton?: boolean,
|};
type State = {|
  isDirty: boolean,
|};

export const setupInstruction = (
  instruction: gdInstruction,
  instructionMetadata: gdInstructionMetadata,
  objectName: ?string
) => {
  instruction.setParametersCount(instructionMetadata.getParametersCount());

  if (objectName) {
    const objectParameterIndex = getObjectParameterIndex(instructionMetadata);
    if (objectParameterIndex === -1) {
      console.error(
        `Instruction "${instructionMetadata.getFullName()}" is used for an object, but does not have an object as first parameter`
      );
      return;
    }

    instruction.setParameter(objectParameterIndex, objectName);
  }
};

const isParameterVisible = (
  parameterMetadata: gdParameterMetadata,
  parameterIndex: number,
  objectParameterIndex: ?number
) => {
  // Hide parameters that are used only for code generation
  if (parameterMetadata.isCodeOnly()) return false;

  // For objects, hide the first object parameter, which is by convention the object name.
  if (parameterIndex === objectParameterIndex) return false;

  return true;
};

export default class InstructionParametersEditor extends React.Component<
  Props,
  State
> {
  _firstVisibleField: ?any = {};
  state = {
    isDirty: false,
  };

  componentDidMount() {
    if (this.props.focusOnMount) {
      setTimeout(() => {
        this.focus();
      }, 300); // Let the time to the dialog that is potentially containing the InstructionParametersEditor to finish its transition.
    }
  }

  focus() {
    const { instruction, isCondition, project } = this.props;

    // Verify that there is a field to focus.
    if (
      this._getVisibleParametersCount(
        getInstructionMetadata({
          instructionType: instruction.getType(),
          isCondition,
          project,
        }),
        this.props.objectName
      ) !== 0
    ) {
      if (this._firstVisibleField && this._firstVisibleField.focus) {
        this._firstVisibleField.focus();
      }
    }
  }

  _getVisibleParametersCount(
    instructionMetadata: ?gdInstructionMetadata,
    objectName: ?string
  ) {
    if (!instructionMetadata) return 0;

    const objectParameterIndex = objectName
      ? getObjectParameterIndex(instructionMetadata)
      : -1;

    return mapFor(0, instructionMetadata.getParametersCount(), i => {
      if (!instructionMetadata) return false;
      const parameterMetadata = instructionMetadata.getParameter(i);

      return isParameterVisible(parameterMetadata, i, objectParameterIndex);
    }).filter(isVisible => isVisible).length;
  }

  _openExtension = (i18n: I18nType) => {
    if (this.state.isDirty) {
      //eslint-disable-next-line
      const answer = confirm(
        i18n._(
          t`You've made some changes here. Are you sure you want to discard them and open the function?`
        )
      );
      if (!answer) return;
    }

    const { instruction, isCondition, project } = this.props;
    const instructionType = instruction.getType();
    if (!instructionType) return null;

    const extension = isCondition
      ? gd.MetadataProvider.getExtensionAndConditionMetadata(
          project.getCurrentPlatform(),
          instructionType
        ).getExtension()
      : gd.MetadataProvider.getExtensionAndActionMetadata(
          project.getCurrentPlatform(),
          instructionType
        ).getExtension();

    this.props.openInstructionOrExpression(extension, instructionType);
  };

  _renderEmpty() {
    return (
      <div style={{ ...styles.emptyContainer, ...this.props.style }}>
        <EmptyMessage>
          {this.props.isCondition
            ? 'Choose a condition (or an object then a condition) on the left'
            : 'Choose an action (or an object then an action) on the left'}
        </EmptyMessage>
      </div>
    );
  }

  render() {
    const {
      instruction,
      project,
      globalObjectsContainer,
      objectsContainer,
      noHelpButton,
      objectName,
      isCondition,
      scope,
    } = this.props;

    const instructionType = instruction.getType();
    const instructionMetadata = getInstructionMetadata({
      instructionType,
      isCondition,
      project,
    });
    if (!instructionMetadata) return this._renderEmpty();

    const helpPage = instructionMetadata.getHelpPath();
    const instructionExtraInformation = getExtraInstructionInformation(
      instructionType
    );
    const objectParameterIndex = objectName
      ? getObjectParameterIndex(instructionMetadata)
      : -1;

    setupInstruction(instruction, instructionMetadata, objectName);

    let parameterFieldIndex = 0;
    return (
      <I18n>
        {({ i18n }) => (
          <div style={styles.container}>
            <Line alignItems="center">
              <img
                src={instructionMetadata.getIconFilename()}
                alt=""
                style={styles.icon}
              />
              <Text>{instructionMetadata.getDescription()}</Text>
              {isAnEventFunctionMetadata(instructionMetadata) && (
                <IconButton
                  onClick={() => {
                    this._openExtension(i18n);
                  }}
                >
                  <OpenInNew />
                </IconButton>
              )}
            </Line>
            {instructionExtraInformation && (
              <Line>
                <AlertMessage kind={instructionExtraInformation.kind}>
                  {i18n._(instructionExtraInformation.message)}
                </AlertMessage>
              </Line>
            )}
            <Spacer />
            <div key={instructionType} style={styles.parametersContainer}>
              {mapFor(0, instructionMetadata.getParametersCount(), i => {
                const parameterMetadata = instructionMetadata.getParameter(i);
                if (
                  !isParameterVisible(
                    parameterMetadata,
                    i,
                    objectParameterIndex
                  )
                )
                  return null;

                const parameterMetadataType = parameterMetadata.getType();
                const ParameterComponent = ParameterRenderingService.getParameterComponent(
                  parameterMetadataType
                );

                // Track the field count on screen, to affect the ref to the
                // first visible field.
                const isFirstVisibleParameterField = parameterFieldIndex === 0;
                parameterFieldIndex++;

                return (
                  <ParameterComponent
                    instructionMetadata={instructionMetadata}
                    instruction={instruction}
                    parameterMetadata={parameterMetadata}
                    parameterIndex={i}
                    value={instruction.getParameter(i)}
                    onChange={value => {
                      if (instruction.getParameter(i) !== value) {
                        instruction.setParameter(i, value);
                        this.setState({
                          isDirty: true,
                        });
                      }
                    }}
                    project={project}
                    scope={scope}
                    globalObjectsContainer={globalObjectsContainer}
                    objectsContainer={objectsContainer}
                    key={i}
                    parameterRenderingService={ParameterRenderingService}
                    resourceSources={this.props.resourceSources}
                    onChooseResource={this.props.onChooseResource}
                    resourceExternalEditors={this.props.resourceExternalEditors}
                    ref={field => {
                      if (isFirstVisibleParameterField) {
                        this._firstVisibleField = field;
                      }
                    }}
                  />
                );
              })}
              {this._getVisibleParametersCount(
                instructionMetadata,
                objectName
              ) === 0 && (
                <EmptyMessage>
                  <Trans>There is nothing to configure.</Trans>
                </EmptyMessage>
              )}
              {this.props.isCondition && (
                <Toggle
                  label={<Trans>Invert condition</Trans>}
                  labelPosition="right"
                  toggled={instruction.isInverted()}
                  style={styles.invertToggle}
                  onToggle={(e, enabled) => {
                    instruction.setInverted(enabled);
                    this.forceUpdate();
                  }}
                />
              )}
            </div>
            <Line>
              {!noHelpButton && helpPage && (
                <HelpButton
                  helpPagePath={instructionMetadata.getHelpPath()}
                  label={
                    this.props.isCondition ? (
                      <Trans>Help for this condition</Trans>
                    ) : (
                      <Trans>Help for this action</Trans>
                    )
                  }
                />
              )}
            </Line>
          </div>
        )}
      </I18n>
    );
  }
}
